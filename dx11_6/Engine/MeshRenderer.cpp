#include "pch.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Game.h"
#include "Mesh.h"
#include "Shader.h"

MeshRenderer::MeshRenderer() : Super(ComponentType::MeshRenderer)
{

}

MeshRenderer::~MeshRenderer()
{

}

//void MeshRenderer::Update()
//{
//	if (_mesh == nullptr || _texture == nullptr || _shader == nullptr)
//		return;
//
//	_shader->GetSRV("Texture0")->SetResource(_texture->GetComPtr().Get());
//
//	auto world = GetTransform()->GetWorldMatrix();
//	RENDER->PushTransformData(TransformDesc{ world });
//	//상수 버퍼에 오브젝트 트랜스폼 데이터 삽입
//	
//	//RENDER->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);
//	//얘는 공용 부분이라 여기서 하긴 이상
//
//	uint32 stride = _mesh->GetVertexBuffer()->GetStride();
//	uint32 offset = _mesh->GetVertexBuffer()->GetOffset();
//
//	DC->IASetVertexBuffers(0, 1, _mesh->GetVertexBuffer()->GetComPtr().GetAddressOf(), &stride, &offset);
//	DC->IASetIndexBuffer(_mesh->GetIndexBuffer()->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
//
//	_shader->DrawIndexed(0, 0, _mesh->GetIndexBuffer()->GetCount(), 0, 0);
//}

void MeshRenderer::Update()
{
	if (_mesh == nullptr || _texture == nullptr || _shader == nullptr)
		return;

	_shader->GetSRV("DiffuseMap")->SetResource(_texture->GetComPtr().Get());

	auto world = GetTransform()->GetWorldMatrix();
	RENDER->PushTransformData(TransformDesc{ world });
	//상수 버퍼에 오브젝트 트랜스폼 데이터 삽입

	//RENDER->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);
	//얘는 공용 부분이라 여기서 하긴 이상

	uint32 stride = _mesh->GetVertexBuffer()->GetStride();
	uint32 offset = _mesh->GetVertexBuffer()->GetOffset();

	DC->IASetVertexBuffers(0, 1, _mesh->GetVertexBuffer()->GetComPtr().GetAddressOf(), &stride, &offset);
	DC->IASetIndexBuffer(_mesh->GetIndexBuffer()->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	_shader->DrawIndexed(0, 0, _mesh->GetIndexBuffer()->GetCount(), 0, 0);
}
