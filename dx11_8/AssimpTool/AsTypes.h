#pragma once

using VertexType = VertexTextureNormalTangentBlendData;

//as �츮�� ���� ����� ���� ����ü
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
	//���� ���� �� ������ �����Ǿ� �ִ°��� ���� ����
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


//�������� -> �� ��ȣ, ����ġ
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

	//����ġ ���� 1�� ������ֱ�
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
	//��� ����ġ �޾� ���� �� ���� �״��� �ʿ��� �Ÿ� ��� (�տ� 4��)
};