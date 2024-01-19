#include "pch.h"
#include "Animator.h"
#include "Game.h"
#include "TimeManager.h"

Animator::Animator()
	: Super(ComponentType::Animator)
{
}

Animator::~Animator()
{
}

void Animator::Init()
{

}

void Animator::Update()
{
	if (_currentAnimation == nullptr)
		return;

	const Keyframe& keyframe = _currentAnimation->GetKeyframe(_currentKeyframeIndex);

	float deltaTime = TIME->GetDeltaTime();
	_sumTime += deltaTime;

	if(_sumTime >= keyframe.time)
	{
		_currentKeyframeIndex++;
		int32 totalCount = _currentAnimation->GetKeyframeCount();
		if (_currentKeyframeIndex >= totalCount)
		{
			if (_currentAnimation->IsLoop())
				_currentKeyframeIndex = 0;
			else
				_currentKeyframeIndex = totalCount - 1;
		}

		_sumTime = 0.f;
	}
}

shared_ptr<Animation> Animator::GetCurrentAnimation()
{
	return _currentAnimation;
}

const Keyframe& Animator::GetCurrnetKeyFrame()
{
	return _currentAnimation->GetKeyframe(_currentKeyframeIndex);
}
