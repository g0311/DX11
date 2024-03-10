#pragma once
#include "Component.h"

enum class ColliderType
{
	Sphere,
	AABB,
	OBB,
};

class BaseCollider : public Component
{
	using Super = Component;
public:
	BaseCollider(ColliderType colliderType);
	virtual ~BaseCollider();

	virtual bool Intersects(Ray& ray, OUT float& distance) = 0;
	virtual bool Intersects(shared_ptr<BaseCollider>& other) = 0;
	//레이저가 닿았는지 여부

	ColliderType GetColliderType() { return _colliderType; }

protected:
	ColliderType _colliderType;
};

