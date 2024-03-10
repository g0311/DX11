#pragma once
#include "BaseCollider.h"

class AABBBoxCollider : public BaseCollider
{
public:
	AABBBoxCollider();
	virtual ~AABBBoxCollider();

	virtual void Update() override;
	virtual bool Intersects(Ray& ray, OUT float& distance) override;
	virtual bool Intersects(shared_ptr<BaseCollider>& other) override;

	BoundingBox& GetBoundingBox() { return _boundingBox; }

private:
	BoundingBox _boundingBox;
	//aabb == Axis Aligned Bounding Box
	//스피어 콜라이더랑 비슷하게 사용됨
};

