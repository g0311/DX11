#pragma once
#include "ResourceBase.h"

class Mesh;
class Material;
class Shader;
class Animation;
class Texture;
class ResourceManager
{
public:
	ResourceManager(ComPtr<ID3D11Device> device);
	~ResourceManager();

	void Init();

	template <typename T>
	shared_ptr<T> Load(const wstring& key, const wstring& path);

	template <typename T>
	bool Add(const wstring& key, shared_ptr<T> obj);

	template <typename T>
	shared_ptr<T> Get(const wstring& key); //Get<Texture>(L"Golem.png") >> texture 번호 >> 2번 맵에서 key로 검색 후 반환

	template <typename T>
	ResourceType GetResourceType();

private:
	void CreateDefaultTexture();
	void CreateDefaultMesh();
	void CreateDefaultShader();
	void CreateDefaultMaterial();
	void CreateDefaultAnimation();

private:

private:
	ComPtr<ID3D11Device> _device;

	using KeyObjMap =  map<wstring/*key*/, shared_ptr<ResourceBase>>;
	array<KeyObjMap, RESOURCE_TYPE_COUNT> _resources; //map 배열
};


template <typename T>
shared_ptr<T> ResourceManager::Load(const wstring& key, const wstring& path)
{
	ResourceType resourceType = GetResourceType<T>();
	KeyObjMap keyObjMap = _resources[static_cast<uint8>(resourceType)];

	auto findIter = keyObjMap.find(key);
	if (findIter != keyObjMap.end())
		return static_pointer_cast<T>(findIter->second);

	shared_ptr<T> obj = make_shared<T>();
	obj->Load(path);
	keyObjMap[key] = obj;

	return obj;
}

template <typename T>
bool ResourceManager::Add(const wstring& key, shared_ptr<T> obj)
{
	ResourceType resourceType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	auto findIter = keyObjMap.find(key);
	if (findIter != keyObjMap.end())
		return false;

	//keyObjMap.insert(pair<wstring, shared_ptr<T>>(key, obj));
	keyObjMap[key] = obj; //<< 자동 삽입 지원

	return true;
}

template <typename T>
shared_ptr<T> ResourceManager::Get(const wstring& key) //Get<Texture>(L"Golem.png") >> texture 번호 >> 2번 맵에서 key로 검색 후 반환
{
	ResourceType resourceType = GetResourceType<T>();
	KeyObjMap keyObjMap = _resources[static_cast<uint8>(resourceType)];
	auto findIter = keyObjMap.find(key);
	if (findIter != keyObjMap.end())
		return static_pointer_cast<T>(findIter->second);

	return nullptr;
}

template <typename T>
ResourceType ResourceManager::GetResourceType()
{
	if (std::is_same_v<T, Mesh>)
		return ResourceType::Mesh;
	if (std::is_same_v<T, Shader>)
		return ResourceType::Shader;
	if (std::is_same_v<T, Texture>)
		return ResourceType::Texture;
	if (std::is_same_v<T, Material>)
		return ResourceType::Material;
	if (std::is_same_v<T, Animation>)
		return ResourceType::Animation;

	assert(false);
	return ResourceType::None;
}
