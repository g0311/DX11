#include "pch.h"
#include "19. InstancingDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"

void InstancingDemo::Init()
{
	RESOURCES->Init();

	_shader = make_shared<Shader>(L"19. InstancingDemo.fx");

	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform()->SetPosition(Vec3{ 0.f, 0.f, -5.f });
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	//Material
	{
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(_shader);
		auto texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\Veigar.jpg");
		material->SetDiffuseMap(texture);
		MaterialDesc& desc = material->GetMaterialDesc();
		desc.ambient = Vec4(1.f);
		desc.diffuse = Vec4(1.f);
		desc.specular = Vec4(1.f);
		RESOURCES->Add(L"Veigar", material);

		//INSTANCING
		_material = material;
	}

	for (int32 i = 0; i < 10000; i++)
	{
		auto obj = make_shared<GameObject>();
		obj->GetOrAddTransform()->SetPosition(Vec3{ (float)(rand() % 100), 0, (float)(rand() % 100) });
		obj->AddComponent(make_shared<MeshRenderer>());
		{
			obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Veigar"));
		}
		{
			auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
			obj->GetMeshRenderer()->SetMesh(mesh);

			//INSTANCING
			_mesh = mesh;
		}
		_objs.push_back(obj);
	}

	RENDER->Init(_shader);
	
	//INSTANCING
	_instanceBuffer = make_shared<VertexBuffer>();

	for(auto& obj : _objs)
	{ 
		Matrix world = obj->GetTransform()->GetWorldMatrix();
		_worlds.push_back(world);
	}
	_instanceBuffer->Create(_worlds, /*slot*/ 1);
}

void InstancingDemo::Update()
{
	_camera->Update();
	RENDER->Update();

	{
		LightDesc lightdesc;
		lightdesc.ambient = Vec4(0.4f);
		lightdesc.diffuse = Vec4(1.f);
		lightdesc.specular = Vec4(0.1f);
		lightdesc.direction = Vec3(1.f, 0.f, 1.f);
		RENDER->PushLightData(lightdesc);
	}

	//for (auto& obj : _objs)
	//{
	//	obj->Update();
	//}
		
	_material->Update();

	{
		//auto world = GetTransform()->GetWorldMatrix();
		//RENDER->PushTransformData(TransformDesc{ world });
		//�̰� ������Ʈ ��Ʋ� ��ü ���� ��Ʈ���� ���ͷ� �����ٰ�
	}
	
	{
		//uint32 stride = _mesh->GetVertexBuffer()->GetStride();
		//uint32 offset = _mesh->GetVertexBuffer()->GetOffset();
		//DC->IASetVertexBuffers(0, 1, _mesh->GetVertexBuffer()->GetComPtr().GetAddressOf(), &stride, &offset);
		_mesh->GetVertexBuffer()->PushData();
		//���ۿ� Ǫ�������� �߰��ؼ� �� ���� ���ص� ��
	}
	_instanceBuffer->PushData();

	//DC->IASetVertexBuffers(1, 1, _mesh->GetVertexBuffer()->GetComPtr().GetAddressOf(), &stride, &offset);
	//DC->IASetIndexBuffer(_mesh->GetIndexBuffer()->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
	_mesh->GetIndexBuffer()->PushData();

	//shader->DrawIndexed(0, 0, _mesh->GetIndexBuffer()->GetCount(), 0, 0);
	_shader->DrawIndexedInstanced(0, 0, _mesh->GetIndexBuffer()->GetCount(), _objs.size());
}

void InstancingDemo::Render()
{

}
