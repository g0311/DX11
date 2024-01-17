#pragma once
#include "Component.h"

class MonoBehaviour : public Component
{
	using Super = Component;
public:
	MonoBehaviour();
	~MonoBehaviour();

	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void FixedUpdate() override;
private:

};

