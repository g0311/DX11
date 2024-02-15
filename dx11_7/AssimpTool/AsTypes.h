#pragma once

using VertexType = VertexTextureNormalTangentBlendData;

//as 우리가 만든 사용자 지정 구조체
struct asBone
{
	string name;
	int32 index = -1;
	int32 parent = -1;
	Matrix transform;
};

struct asMesh
{
	string name;
	aiMesh* mesh;
	vector<VertexType> verices;
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
