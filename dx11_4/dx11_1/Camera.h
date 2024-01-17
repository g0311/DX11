#pragma once
#include "Component.h"

enum class ProjectionType
{
	Perspective, //원근 투영
	Orthographic, //직교 투영
};

class Camera : public Component
{
	using Super = Component;
public:
	Camera();
	virtual ~Camera();

	virtual void Update() override;

	void SetProjectionType(ProjectionType type) { _type = type; }
	ProjectionType GetProjectionType() { return _type; }

	void UpdateMatrix();
	//뷰 매트릭스, 프로젝션 매트릭스 생성
private:
	ProjectionType _type = ProjectionType::Orthographic;

public:
	static Matrix S_MatView;
	static Matrix S_MatProjection;
};

