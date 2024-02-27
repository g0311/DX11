#include "pch.h"
#include "Converter.h"
#include <filesystem>
#include "Utils.h"
#include "tinyxml2.h"
#include "FileUtils.h"

Converter::Converter()
{
	_importer = make_shared<Assimp::Importer>();
}

Converter::~Converter()
{

}

void Converter::ReadAssetFile(wstring file)
{
	wstring fileStr = _assetPath + file;
	
	auto p = std::filesystem::path(fileStr);
	assert(std::filesystem::exists(p));

	_scene = _importer->ReadFile(
		Utils::ToString(fileStr),
		aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate |
		aiProcess_GenUVCoords |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	assert(_scene != nullptr);
}

void Converter::ExportModelData(wstring savePath)
{
	wstring finalPath = _modelPath + savePath + L".mesh";//바이너리라 못읽음
	ReadModelData(_scene->mRootNode, -1, -1); //루트 노드는 가상의 노드래
	ReadSkinData();

	//CSV
	{
		FILE* file;
		::fopen_s(&file, "../Vertices.csv", "w");

		for (shared_ptr<asBone>& bone : _bones)
		{
			string name = bone->name;
			::fprintf(file, "%d,%s\n", bone->index, bone->name.c_str());
		}

		::fprintf(file, "\n");

		for (shared_ptr<asMesh>& mesh : _meshes)
		{
			string name = mesh->name;
			::fprintf(file, "%s\n", name.c_str());

			for (UINT i = 0; i < mesh->vertices.size(); i++)
			{
				Vec3 p = mesh->vertices[i].position;
				Vec4 indices = mesh->vertices[i].blendIndices;
				Vec4 weights = mesh->vertices[i].blendWeights;

				::fprintf(file, "%f,%f,%f,", p.x, p.y, p.z);
				::fprintf(file, "%f,%f,%f,%f,", indices.x, indices.y, indices.z, indices.w);
				::fprintf(file, "%f,%f,%f,%f\n", weights.x, weights.y, weights.z, weights.w);
			}
		}
		::fclose(file);
	}

	WriteModelFile(finalPath);
}

void Converter::ExportMaterialData(wstring savePath)
{
	wstring finalPath = _texturePath + savePath + L".xml";//xml이라 열 수 있음
	ReadMaterialData();
	WriteMaterialData(finalPath);
}

void Converter::ExportAnimationData(wstring savePath, uint32 index)
{
	wstring finalPath = _modelPath + savePath + L".clip";
	assert(index < _scene->mNumAnimations);

	shared_ptr<asAnimation> animation = ReadAnimationData(_scene->mAnimations[index]);
	WriteAnimationData(animation, finalPath);
}

void Converter::ReadModelData(aiNode* node, int32 index, int32 parent)//재귀 호출
{
	shared_ptr<asBone> bone = make_shared<asBone>();
	bone->index = index;
	bone->parent = parent;
	bone->name = node->mName.C_Str();

	//Relative Transform
	Matrix transform(node->mTransformation[0]); //그냥첫번째주소던지면댐
	bone->transform = transform.Transpose(); //원래 열우선 //상대 좌표

	//Parent Local Transform
	Matrix matParent = Matrix::Identity;
	if (parent >= 0)
		matParent = _bones[parent]->transform;

	//Local Transform
	bone->transform *= matParent;
	_bones.push_back(bone);

	//Mesh
	ReadMeshData(node, index);

	//재귀
	for (int i = 0; i < node->mNumChildren; i++)
	{
		ReadModelData(node->mChildren[i], _bones.size(), index);
	}
}

void Converter::ReadMeshData(aiNode* node, int32 bone)
{
	if (node->mNumMeshes < 1)
		return;

	shared_ptr<asMesh> mesh = make_shared<asMesh>();
	mesh->name = node->mName.C_Str();
	mesh->boneIndex = bone;

	for (uint32 i = 0; i < node->mNumMeshes; i++)
	{
		uint32 index = node->mMeshes[i];
		//이게 씬의 메시 번호 (노드가 씬에서 접근하기 위한 인덱스)
		const aiMesh* srcMesh = _scene->mMeshes[index];
		//머티리얼 정보에 대해서도 가지고 있음 >> 우리가 메시에 안넣어줘도 된대

		//Material Name
		const aiMaterial* material = _scene->mMaterials[srcMesh->mMaterialIndex];
		mesh->materialName = material->GetName().C_Str();
		//이걸 반복문 안에서 수행하면 마지막 서브 메시의 머티리얼이 들어가는게 아닌가?

		const uint32 startVertex = mesh->vertices.size();

		for (uint32 v = 0; v < srcMesh->mNumVertices; v++)
		{
			//Vertex
			VertexType vertex;
			::memcpy(&vertex.position, &srcMesh->mVertices[v], sizeof(Vec3));

			//UV
			if (srcMesh->HasTextureCoords(0))
				::memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Vec2));

			//Normal
			if (srcMesh->HasNormals())
				::memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Vec3));

			mesh->vertices.push_back(vertex);
		}

		//Index
		for (uint32 f = 0; f < srcMesh->mNumFaces; f++)
		{
			aiFace& face = srcMesh->mFaces[f];

			for (uint32 k = 0; k < face.mNumIndices; k++)
				mesh->indices.push_back(face.mIndices[k] + startVertex);
		}
	}
	_meshes.push_back(mesh);
}

void Converter::ReadSkinData()
{
	for (uint32 i = 0; i < _scene->mNumMeshes; i++)
	{
		aiMesh* srcMesh = _scene->mMeshes[i];
		if (srcMesh->HasBones() == false)
			continue;

		shared_ptr<asMesh> mesh = _meshes[i];

		//정점마다 -> 뼈 번호, 가중치
		vector<asBoneWeights> tempVertexBoneWeights;
		tempVertexBoneWeights.resize(mesh->vertices.size());

		//메쉬와 관련된 bone을 순회하면서 연관된 vertexid, weight를 구해서 저장
		for (uint32 j = 0; j < srcMesh->mNumBones; j++)
		{
			//메시 >> 뼈 >> 정점
			aiBone* srcMeshBone = srcMesh->mBones[j];
			//해당 뼈의 인덱스
			uint32 boneIndex = GetBoneIndex(srcMeshBone->mName.C_Str());
			for (uint32 w = 0; w < srcMeshBone->mNumWeights; w++)
			{
				uint32 index = srcMeshBone->mWeights[w].mVertexId; //메시가 가지는 정점 번호
				float weight = srcMeshBone->mWeights[w].mWeight;
				//인덱스 << 해당 메시의 정점 번호 획득
				/*
				메시에 정점이 존재한다
				메시는 연관된 본을 가지고 있다
				본은 메시의 정점에 대해서 어떤 가중치를 줘야하는지 정보를 가지고있다
				*/
				tempVertexBoneWeights[index].AddWeights(boneIndex, weight);
			}
		}
		//결과 계산
		for (uint32 v = 0; v < tempVertexBoneWeights.size(); v++)
		{
			tempVertexBoneWeights[v].Normalize();
			asBlendWeight blendWeight = tempVertexBoneWeights[v].GetBlendWeights();
			mesh->vertices[v].blendIndices = blendWeight.indices;
			mesh->vertices[v].blendWeights = blendWeight.weights;
		}
	}
}

void Converter::WriteModelFile(wstring finalPath)
{
	//디렉터리 존재x 시 생성
	auto path = filesystem::path(finalPath);
	filesystem::create_directory(path.parent_path());

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(finalPath, FileMode::Write);
	
	//Bones
	file->Write<uint32>(_bones.size());
	for (shared_ptr<asBone>& bone : _bones)
	{
		file->Write<int32>(bone->index);
		file->Write<string>(bone->name);
		file->Write<int32>(bone->parent);
		file->Write<Matrix>(bone->transform);
	}

	//Meshes
	file->Write<uint32>(_meshes.size());
	for (shared_ptr<asMesh>& meshData : _meshes)
	{
		file->Write<string>(meshData->name);
		file->Write<int32>(meshData->boneIndex);
		file->Write<string>(meshData->materialName);

		// Vertex Data
		file->Write<uint32>(meshData->vertices.size());
		file->Write(&meshData->vertices[0], sizeof(VertexType) * meshData->vertices.size());

		// Index Data
		file->Write<uint32>(meshData->indices.size());
		file->Write(&meshData->indices[0], sizeof(uint32) * meshData->indices.size());
	}
}

void Converter::ReadMaterialData()
{
	for (uint32 i = 0; i < _scene->mNumMaterials; i++)
	{
		aiMaterial* srcMaterial = _scene->mMaterials[i];
		shared_ptr<asMaterial> material = make_shared<asMaterial>();
		material->name = srcMaterial->GetName().C_Str();
		
		aiColor3D color;
		{
			srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
			material->ambient = Color(color.r, color.g, color.b, 1.f);

			srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			material->diffuse = Color(color.r, color.g, color.b, 1.f);

			srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
			material->specular = Color(color.r, color.g, color.b, 1.f);
			srcMaterial->Get(AI_MATKEY_SHININESS, material->specular.w);

			srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
			material->emissive = Color(color.r, color.g, color.b, 1.f);
		}

		aiString file;
		{
			srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
			material->diffuseFile = file.C_Str();

			srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
			material->specularFile = file.C_Str();

			srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
			material->normalFile = file.C_Str();
		}
		_materials.push_back(material);
	}
}

void Converter::WriteMaterialData(wstring finalPath)
{
	auto path = filesystem::path(finalPath);

	//폴더가 없을 시
	filesystem::create_directory(path.parent_path());

	string folder = path.parent_path().string();

	shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();
	
	tinyxml2::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	tinyxml2::XMLElement* root = document->NewElement("Materials");
	document->LinkEndChild(root);
	
	for (shared_ptr<asMaterial> material : _materials)
	{
		tinyxml2::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);

		tinyxml2::XMLElement* element = document->NewElement("Name");
		element->SetText(material->name.c_str());
		node->LinkEndChild(element);

		//텍스쳐 파일 복사 후 텍스쳐 폴더에 저장
		//복사된 파일 위치 머티리얼에 저장
		element = document->NewElement("DiffuseFile");
		element->SetText(WriteTexture(folder, material->diffuseFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("SpecularFile");
		element->SetText(WriteTexture(folder, material->specularFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("NormalFile");
		element->SetText(WriteTexture(folder, material->normalFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("Ambient");
		element->SetAttribute("R", material->ambient.x);
		element->SetAttribute("G", material->ambient.y);
		element->SetAttribute("B", material->ambient.z);
		element->SetAttribute("A", material->ambient.w);
		node->LinkEndChild(element);

		element = document->NewElement("Diffuse");
		element->SetAttribute("R", material->diffuse.x);
		element->SetAttribute("G", material->diffuse.y);
		element->SetAttribute("B", material->diffuse.z);
		element->SetAttribute("A", material->diffuse.w);
		node->LinkEndChild(element);

		element = document->NewElement("Specular");
		element->SetAttribute("R", material->specular.x);
		element->SetAttribute("G", material->specular.y);
		element->SetAttribute("B", material->specular.z);
		element->SetAttribute("A", material->specular.w);
		node->LinkEndChild(element);

		element = document->NewElement("Emissive");
		element->SetAttribute("R", material->emissive.x);
		element->SetAttribute("G", material->emissive.y);
		element->SetAttribute("B", material->emissive.z);
		element->SetAttribute("A", material->emissive.w);
		node->LinkEndChild(element);
	}
	document->SaveFile(Utils::ToString(finalPath).c_str());
}

std::string Converter::WriteTexture(string saveFolder, string file)
{
	string fileName = filesystem::path(file).filename().string();
	string folderName = filesystem::path(saveFolder).filename().string();

	const aiTexture* srcTexture = _scene->GetEmbeddedTexture(file.c_str());
	if (srcTexture) //fbx 파일에 텍스쳐가 저장되어 있을 시
	{
		string pathStr = (filesystem::path(saveFolder) / folderName).string();

		if (srcTexture->mHeight == 0)
		{
			shared_ptr<FileUtils> file = make_shared<FileUtils>();
			file->Open(Utils::ToWString(pathStr), FileMode::Write);
			file->Write(srcTexture->pcData, srcTexture->mWidth);
		}
		else
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
			desc.Width = srcTexture->mWidth;
			desc.Height = srcTexture->mHeight;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_IMMUTABLE;

			D3D11_SUBRESOURCE_DATA subResource = { 0 };
			subResource.pSysMem = srcTexture->pcData;

			ComPtr<ID3D11Texture2D> texture;
			HRESULT hr = DEVICE->CreateTexture2D(&desc, &subResource, texture.GetAddressOf());
			CHECK(hr);

			DirectX::ScratchImage img;
			::CaptureTexture(DEVICE.Get(), DC.Get(), texture.Get(), img);

			// Save To File
			hr = DirectX::SaveToDDSFile(*img.GetImages(), DirectX::DDS_FLAGS_NONE, Utils::ToWString(fileName).c_str());
			CHECK(hr);
		}
	}
	else
	{
		//filesystem에서 /연산자가 그냥 경로로 파악하게 지원을 함
		string originStr = (filesystem::path(_assetPath) / folderName / file).string();
		Utils::Replace(originStr, "\\", "/");

		string pathStr = (filesystem::path(saveFolder) / fileName).string();
		Utils::Replace(pathStr, "\\", "/");

		::CopyFileA(originStr.c_str(), pathStr.c_str(), false);
	}
	return fileName;
}

shared_ptr<asAnimation> Converter::ReadAnimationData(aiAnimation* srcAnimation)
{
	shared_ptr<asAnimation> animation = make_shared<asAnimation>();
	animation->name = srcAnimation->mName.C_Str();
	animation->frameRate = (float)srcAnimation->mTicksPerSecond;
	animation->frameCount = srcAnimation->mDuration + 1; //프레임 갯수라 1추가 (0부터 시작)

	map<string, shared_ptr<asAnimationNode>> cacheAnimation;
	//이게 뭐하는 애지..

	for (uint32 i = 0; i < srcAnimation->mNumChannels; i++) //본 개수
	{
		aiNodeAnim* srcNode = srcAnimation->mChannels[i];

		//애니메이션 노드 데이터 파싱
		shared_ptr<asAnimationNode> node = ParseAnimationNode(animation, srcNode);

		//현재 찾은 노드 중 제일 긴 시간으로 애니메이션 시간 갱신
		animation->duration = max(animation->duration, node->keyframe.back().time);

		cacheAnimation[srcNode->mNodeName.C_Str()] = node;
	}
	ReadKeyframeData(animation, _scene->mRootNode, cacheAnimation);
	return animation;
}

shared_ptr<asAnimationNode> Converter::ParseAnimationNode(shared_ptr<asAnimation> animation, aiNodeAnim* srcNode)
{
	//srt 정보 저장
	shared_ptr<asAnimationNode> node = make_shared<asAnimationNode>();
	node->name = srcNode->mNodeName;

	uint32 keyCount = max(max(srcNode->mNumPositionKeys, srcNode->mNumRotationKeys), srcNode->mNumScalingKeys);
	
	for (uint32 k = 0; k < keyCount; k++)
	{
		asKeyframeData frameData;

		bool found = false;
		uint32 t = node->keyframe.size();

		// Position
		if (::fabsf((float)srcNode->mPositionKeys[k].mTime - (float)t) <= 0.0001f)
		{
			aiVectorKey key = srcNode->mPositionKeys[k];
			frameData.time = (float)key.mTime;
			::memcpy_s(&frameData.translation, sizeof(Vec3), &key.mValue, sizeof(aiVector3D));

			found = true;
		}

		// Rotation
		if (::fabsf((float)srcNode->mRotationKeys[k].mTime - (float)t) <= 0.0001f)
		{
			aiQuatKey key = srcNode->mRotationKeys[k];
			frameData.time = (float)key.mTime;

			frameData.rotation.x = key.mValue.x;
			frameData.rotation.y = key.mValue.y;
			frameData.rotation.z = key.mValue.z;
			frameData.rotation.w = key.mValue.w;

			found = true;
		}

		// Scale
		if (::fabsf((float)srcNode->mScalingKeys[k].mTime - (float)t) <= 0.0001f)
		{
			aiVectorKey key = srcNode->mScalingKeys[k];
			frameData.time = (float)key.mTime;
			::memcpy_s(&frameData.scale, sizeof(Vec3), &key.mValue, sizeof(aiVector3D));

			found = true;
		}

		if (found == true)
			node->keyframe.push_back(frameData);
	}

	// Keyframe 늘려주기
	if (node->keyframe.size() < animation->frameCount)
	{
		uint32 count = animation->frameCount - node->keyframe.size();
		asKeyframeData keyFrame = node->keyframe.back();

		for (uint32 n = 0; n < count; n++)
			node->keyframe.push_back(keyFrame);
	}

	return node;
}

void Converter::ReadKeyframeData(shared_ptr<asAnimation> animation, aiNode* srcNode, map<string, shared_ptr<asAnimationNode>>& cache)
{//얘도 재귀적으로 호출됨
	shared_ptr<asKeyframe> keyframe = make_shared<asKeyframe>();
	keyframe->boneName = srcNode->mName.C_Str();

	shared_ptr<asAnimationNode> findNode = cache[srcNode->mName.C_Str()];

	for (uint32 i = 0; i < animation->frameCount; i++)
	{
		asKeyframeData frameData;

		if (findNode == nullptr)
		{
			Matrix transform(srcNode->mTransformation[0]);
			transform = transform.Transpose();
			frameData.time = (float)i;
			transform.Decompose(OUT frameData.scale, OUT frameData.rotation, OUT frameData.translation);
		}
		else
		{
			frameData = findNode->keyframe[i];
		}

		keyframe->transforms.push_back(frameData);
	}
	animation->keyframes.push_back(keyframe);
	
	for (uint32 i = 0; i < srcNode->mNumChildren; i++)
		ReadKeyframeData(animation, srcNode->mChildren[i], cache);
}

void Converter::WriteAnimationData(shared_ptr<asAnimation> animation, wstring finalPath)
{
	auto path = filesystem::path(finalPath);

	//폴더 없을 시 생성
	filesystem::create_directory(path.parent_path());

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(finalPath, FileMode::Write);
	
	file->Write<string>(animation->name);
	file->Write<float>(animation->duration);
	file->Write<float>(animation->frameRate);
	file->Write<uint32>(animation->frameCount);

	file->Write<uint32>(animation->keyframes.size());

	for (shared_ptr<asKeyframe> keyframe : animation->keyframes)
	{
		file->Write<string>(keyframe->boneName);

		file->Write<uint32>(keyframe->transforms.size());
		file->Write(&keyframe->transforms[0], sizeof(asKeyframeData) * keyframe->transforms.size());
	}
}

uint32 Converter::GetBoneIndex(const string& name)
{
	for (shared_ptr<asBone>& bone : _bones)
	{
		if (bone->name == name)
			return bone->index;
	}
	assert(false);
	return 0;
}
