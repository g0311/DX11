#include "pch.h"
#include "ModelRenderer.h"
#include "Model.h"
#include "ModelMesh.h"
#include "Shader.h"
#include "Material.h"

ModelRenderer::ModelRenderer(shared_ptr<Shader> shader)
	: Super(ComponentType::ModelRenderer), _shader(shader)
{
}

ModelRenderer::~ModelRenderer()
{
}

//void ModelRenderer::Update()
//{
//	if (_model == nullptr)
//		return;
//
//	auto world = GetTransform()->GetWorldMatrix();
//	RENDER->PushTransformData(TransformDesc{ world });
//
//	const auto& meshes = _model->GetMeshes();
//	for (auto& mesh : meshes)
//	{
//		if (mesh->material)
//			mesh->material->Update();
//
//		uint32 stride = mesh->vertexBuffer->GetStride();
//		uint32 offset = mesh->vertexBuffer->GetOffset();
//
//		DC->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
//		DC->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
//
//		_shader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
//	}
//}

//void ModelRenderer::Update()
//{
//	if (_model == nullptr)
//		return;
//
//	//Bones
//	BoneDesc boneDesc;
//	const uint32 boneCount = _model->GetBoneCount();
//	for (uint32 i = 0; i < boneCount; i++)
//	{
//		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
//		boneDesc.Transforms[i] = bone->transform;
//	}
//	RENDER->PushBoneData(boneDesc);
//
//	//Transform
//	auto world = GetTransform()->GetWorldMatrix();
//	RENDER->PushTransformData(TransformDesc{ world });
//
//	const auto& meshes = _model->GetMeshes();
//	for (auto& mesh : meshes)
//	{
//		if (mesh->material)
//			mesh->material->Update();
//
//		//Bone Index
//		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);
//
//		uint32 stride = mesh->vertexBuffer->GetStride();
//		uint32 offset = mesh->vertexBuffer->GetOffset();
//
//		DC->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
//		DC->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
//
//		_shader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
//	}
//}

void ModelRenderer::SetModel(shared_ptr<Model> model)
{
	_model = model;

	const auto& materials = _model->GetMaterials();
	for (auto& material : materials)
	{
		material->SetShader(_shader);
	}
}

void ModelRenderer::RenderInstancing(shared_ptr<class InstancingBuffer>& buffer)
{
	if (_model == nullptr)
		return;

	//Bones
	BoneDesc boneDesc;
	const uint32 boneCount = _model->GetBoneCount();
	for (uint32 i = 0; i < boneCount; i++)
	{
		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
		boneDesc.Transforms[i] = bone->transform;
	}
	RENDER->PushBoneData(boneDesc);

	const auto& meshes = _model->GetMeshes();
	for (auto& mesh : meshes)
	{
		if (mesh->material)
			mesh->material->Update();

		//Bone Index
		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		mesh->vertexBuffer->PushData();
		mesh->indexBuffer->PushData();
		
		buffer->PushData();

		//_shader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
		_shader->DrawIndexedInstanced(0, _pass, mesh->indexBuffer->GetCount(), buffer->GetCount());
		//각 메시에 대해서 인스턴싱???
			//오브젝트 갯수와는 상관 없이 부품 갯수만큼만 드로우콜이 호출 될것
	}
}

InstanceID ModelRenderer::GetInstanceID()
{
	return make_pair((uint64)_model.get(), (uint64)_shader.get());
}
