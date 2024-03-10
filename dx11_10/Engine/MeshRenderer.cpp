#include "pch.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Game.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Light.h"

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

//void MeshRenderer::Update()
//{
//	if (_mesh == nullptr || _texture == nullptr || _shader == nullptr)
//		return;
//
//	_shader->GetSRV("DiffuseMap")->SetResource(_texture->GetComPtr().Get());
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

//void MeshRenderer::Update()
//{
//	if (_mesh == nullptr || _material == nullptr)
//		return;
//
//	auto shader = _material->GetShader();
//	if (shader == nullptr)
//		return;
//
//	_material->Update(); //머티리얼 정보 (텍스쳐)쉐이더로 푸쉬
//	//얘를 왜 해주는거임?? 이해가 안가네
//
//	auto world = GetTransform()->GetWorldMatrix();
//	RENDER->PushTransformData(TransformDesc{ world });
//	//상수 버퍼에 오브젝트 트랜스폼 데이터 삽입
//
//	//RENDER->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);
//	//얘는 공용 부분이라 여기서 하긴 이상
//	//렌더 매니져에서 수행
//
//	uint32 stride = _mesh->GetVertexBuffer()->GetStride();
//	uint32 offset = _mesh->GetVertexBuffer()->GetOffset();
//
//	DC->IASetVertexBuffers(0, 1, _mesh->GetVertexBuffer()->GetComPtr().GetAddressOf(), &stride, &offset);
//	DC->IASetIndexBuffer(_mesh->GetIndexBuffer()->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
//
//	shader->DrawIndexed(0, 0, _mesh->GetIndexBuffer()->GetCount(), 0, 0);
//}

void MeshRenderer::RenderInstancing(shared_ptr<class InstancingBuffer>& buffer)
{
	if (_mesh == nullptr || _material == nullptr)
		return;

	auto shader = _material->GetShader();
	if (shader == nullptr)
		return;

	// GlobalData
	shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

	// Light
	auto lightObj = SCENE->GetCurrentScene()->GetLight();
	if (lightObj)
		shader->PushLightData(lightObj->GetLight()->GetLightDesc());

	//LIGHT
	_material->Update(); //머티리얼 정보 (텍스쳐)쉐이더로 푸쉬

	//IA
	_mesh->GetVertexBuffer()->PushData();
	_mesh->GetIndexBuffer()->PushData(); //지오메트리 푸쉬

	//INSTANCING
	buffer->PushData();//월드 매트릭스 푸쉬

	//DRAW
	shader->DrawIndexedInstanced(0, _pass, _mesh->GetIndexBuffer()->GetCount(), buffer->GetCount());
}

InstanceID MeshRenderer::GetInstanceID()
{
	return make_pair((uint64)_mesh.get(), (uint64)_material.get());
}
