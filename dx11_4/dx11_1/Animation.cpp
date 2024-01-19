#include "pch.h"
#include "Animation.h"
#include "Texture.h"

Animation::Animation()
	:Super(ResourceType::Animation)
{
}

Animation::~Animation()
{
}

void Animation::Load(const wstring& path)
{
	tinyxml2::XMLDocument doc; 
	string pathStr(path.begin(), path.end());
	XMLError result = doc.LoadFile(pathStr.c_str()); //문서 읽기
	assert(result == XMLError::XML_SUCCESS);

	XMLElement* root = doc.FirstChildElement(); //첫 원소 읽기
	string nameStr = root->Attribute("Name");
	_name = wstring(nameStr.begin(), nameStr.end());
	_loop = root->BoolAttribute("Loop");
	_path = path;

	//Load Texture
	XMLElement* node = root->FirstChildElement(); //루트의 첫 원소 읽기
	for(; node != nullptr; node = node->NextSiblingElement()) //다음 원소 읽기
	{
		Keyframe keyframe;
		keyframe.offset.x = node->FloatAttribute("OffsetX");
		keyframe.offset.y = node->FloatAttribute("OffsetY"); //성분 읽어오기
		keyframe.size.x = node->FloatAttribute("SizeX");
		keyframe.size.y = node->FloatAttribute("SizeY");
		keyframe.time = node->FloatAttribute("Time");

		AddKeyframe(keyframe);
	}
}

void Animation::Save(const wstring& path)
{
	tinyxml2::XMLDocument doc; //문서
	XMLElement* root = doc.NewElement("Animation"); //원소 추가
	doc.LinkEndChild(root);

	string nameStr(GetName().begin(), GetName().end());
	root->SetAttribute("Name", nameStr.c_str()); //성분 추가
	root->SetAttribute("Loop", _loop);
	root->SetAttribute("Path", "TODO");

	for (const Keyframe& keyframe : _keyframes)
	{
		XMLElement* node = doc.NewElement("Keyframe"); //원소 추가
		root->LinkEndChild(node); //애기 노드 연결

		node->SetAttribute("OffsetX", keyframe.offset.x);
		node->SetAttribute("OffsetY", keyframe.offset.y);
		node->SetAttribute("SizeX", keyframe.size.x);
		node->SetAttribute("SizeY", keyframe.size.y);
		node->SetAttribute("Time", keyframe.time);
	}
	string pathStr(path.begin(), path.end());
	XMLError result = doc.SaveFile(pathStr.c_str()); //문서 저장
	assert(result == XMLError::XML_SUCCESS);
}

Vec2 Animation::GetTextureSize()
{ 
	return _texture->GetSize();
}

const Keyframe& Animation::GetKeyframe(int32 index)
{
	return _keyframes[index];
}

int32 Animation::GetKeyframeCount()
{
	return static_cast<int32>(_keyframes.size());
}

void Animation::AddKeyframe(const Keyframe& keyframe)
{
	_keyframes.push_back(keyframe);
}
