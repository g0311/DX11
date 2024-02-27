#pragma once

struct ModelKeyframeData
{
	float time;
	Vec3 scale;
	Quaternion rotation;
	Vec3 translation;
};

struct ModelKeyframe
{
	wstring boneName;
	vector<ModelKeyframeData> transforms;//프레임 개수
};


struct ModelAnimation
{
	shared_ptr<ModelKeyframe> GetKeyframe(const wstring& name);

	wstring name;
	float duration = 0.f;
	float frameRate = 0.f;
	uint32 frameCount = 0;
	unordered_map<wstring, shared_ptr<ModelKeyframe>> keyframes;
	//왜 해쉬 맵?? >> 뼈 이름으로 키프레임 찾아야할 필요가 있기 때문
};

