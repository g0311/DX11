#include "pch.h"
#include "Camera.h"
#include "GameObject.h"

Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;

Camera::Camera()
	:Super(ComponentType::Camera)
{
}

Camera::~Camera()
{
}

void Camera::Update()
{
	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	//S_MatView = GetTransform()->GetWorldMatrix().Invert();
	Vec3 eyePosition = GetTransform()->GetPosition();
	Vec3 focusPosition = eyePosition + GetTransform()->GetLook();
	Vec3 upDirection = GetTransform()->GetUp();
	S_MatView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);

 	if (_type == ProjectionType::Perspective)
		S_MatProjection = ::XMMatrixPerspectiveFovLH(XM_PI/3.f, 800.f/600.f, 1.f, 100.f);
	else
		S_MatProjection = ::XMMatrixOrthographicLH(8.f, 6.f, 0.f, 1.f);
}
