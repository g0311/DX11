#include "pch.h"
#include "Transform.h"

Transform::Transform()
	: Super(ComponentType::Transform)
{
}

Transform::~Transform()
{
}

void Transform::Awake()
{
}

void Transform::Update()
{
}

Vec3 ToEulerAngles(Quaternion q)
{
	Vec3 angles;

	// roll (x-axis rotation)
	double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
	double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
	angles.y = 2 * std::atan2(sinp, cosp) - 3.14159f / 2;

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}

void Transform::UpdateTransform()
{
	Matrix matScale = Matrix::CreateScale(_localScale);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	_matLocal = matScale * matRotation * matTranslation;
	if (HasParent())
	{
		_matWorld = _matLocal * _parent->GetWorldMatrix();
	}
	else
	{
		_matWorld = _matLocal;
	}
	
	Quaternion quat;
	_matWorld.Decompose(_scale, quat, _position);
	//좌표계 변환 행렬에서 srt 뽑아오는 함수
	_rotation = ToEulerAngles(quat);

	//TransformCoord (위치 포함)
	//TransformNormal (방향만)

	//children
	for (const shared_ptr<Transform>& child : _children)
	{
		child->UpdateTransform();
	}
}

void Transform::SetScale(const Vec3& worldScale)
{
	if (HasParent())
	{
		Vec3 parentScale = _parent->GetScale();
		Vec3 scale = worldScale;
		scale.x /= parentScale.x;
		scale.y /= parentScale.y;
		scale.z /= parentScale.z;
		SetLocalScale(scale);
	}
	else
	{
		SetLocalScale(worldScale);
	}
}

void Transform::SetRotation(const Vec3& worldRotation)
{
	if (HasParent())
	{
		Matrix worldToParentLocalMatrix = _parent->GetWorldMatrix().Invert();
		Vec3 rotation;
		rotation.TransformNormal(worldRotation, worldToParentLocalMatrix);
		SetLocalRotation(rotation);
	}
	else
	{
		SetLocalRotation(worldRotation);
	}
}

void Transform::SetPosition(const Vec3& worldPosition)
{
	if (HasParent())
	{
		//Vec3 parentPosition = _parent->GetPosition();
		//Vec3 position = worldPosition;
		//position.x -= parentPosition.x; ...y...z...
		//이렇게 하면 안댐 (rotation 적용 되어 있을 시 사고)
		//로테이션 변환 역행렬 적용 후 감산? ㄴㄴ 월드 변환 행렬의 역을 사용
		Matrix worldToParentLocalMatrix = _parent->GetWorldMatrix().Invert();
		Vec3 position;
		position.Transform(worldPosition, worldToParentLocalMatrix);
		SetLocalPosition(position);
	}
	else
	{
		SetLocalPosition(worldPosition);
	}
}
