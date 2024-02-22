#pragma once

using VertexType = VertexTextureNormalTangentBlendData;

//as 우리가 만든 사용자 지정 구조체
struct asBone
{
	string name;
	int32 index = -1;
	int32 parent = -1;
	Matrix transform; //SRT
};

struct asMesh
{
	string name;
	aiMesh* mesh;
	vector<VertexType> vertices;
	vector<uint32> indices;

	int32 boneIndex;
	//계층 구조 내 누구와 연관되어 있는가에 대한 정보
	string materialName;
};

struct asMaterial
{
	string name;
	Color ambient;
	Color diffuse;
	Color specular;
	Color emissive;
	string diffuseFile;
	string specularFile;
	string normalFile;
};

//Animation
struct asBlendWeight
{
	void Set(uint32 index, uint32 boneIndex, float weight)
	{
		float i = (float)boneIndex;
		float w = weight;

		switch (index)
		{
		case 0: indices.x = i; weights.x = w; break;
		case 1: indices.y = i; weights.y = w; break;
		case 2: indices.z = i; weights.z = w; break;
		case 3: indices.w = i; weights.w = w; break;
		}
	}

	Vec4 indices = Vec4(0, 0, 0, 0);
	Vec4 weights = Vec4(0, 0, 0, 0);
};


//정점마다 -> 뼈 번호, 가중치
struct asBoneWeights
{
	void AddWeights(uint32 boneindex, float weight)
	{
		if (weight <= 0.0f)
			return;
		
		auto findIt = std::find_if(boneWeights.begin(), boneWeights.end(),
			[weight](const Pair& p) {return weight > p.second; });
		
		// (1, 0.4), (2, 0.2) << (5, 0.4)
		boneWeights.insert(findIt, Pair(boneindex, weight));
	}

	asBlendWeight GetBlendWeight()
	{
		asBlendWeight blendWeights;

		for (uint32 i = 0; i < boneWeights.size(); i++)
		{
			if (i >= 4)
				break;

			blendWeights.Set(i, boneWeights[i].first, boneWeights[i].second);
		}

		return blendWeights;
	}

	//가중치 합을 1로 만들어주기
	void Normalize()
	{
		if (boneWeights.size() >= 4)
			boneWeights.resize(4);

		float totalWeight = 0.f;
		for (const auto& item : boneWeights)
			totalWeight += item.second;

		float scale = 1.f / totalWeight;

		for (auto& item : boneWeights)
			item.second *= scale;
	}

	using Pair = pair<int32, float>;
	vector<Pair> boneWeights;
	//모든 가중치 받아 저장 후 정렬 그다음 필요한 거만 사용 (앞에 4개)
};