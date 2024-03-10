#include "pch.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"
#include "Light.h"
#include "CollisionDemo.h"
#include "TextureBuffer.h"
#include "Viewport.h"
#include "SphereCollider.h"
#include "AABBBoxCollider.h"
#include "OBBBoxCollider.h"
#include "Scene.h"
#include "Terrain.h"

void CollisionDemo::Init()
{
	_shader = make_shared<Shader>(L"23. RenderDemo.fx");

	//camera
	{
		auto camera = make_shared<GameObject>();
		camera->GetOrAddTransform()->SetPosition(Vec3{ 0.f, 0.f, -5.f });
		camera->AddComponent(make_shared<Camera>());
		camera->AddComponent(make_shared<CameraScript>());
		
		CUR_SCENE->Add(camera);
	}
	//Light
	{
		auto light = make_shared<GameObject>();
		light->GetOrAddTransform()->SetPosition(Vec3{ 0.f, 0.f, -5.f });
		light->AddComponent(make_shared<Light>());

		LightDesc lightdesc;
		lightdesc.ambient = Vec4(0.4f);
		lightdesc.diffuse = Vec4(1.f);
		lightdesc.specular = Vec4(0.1f);
		lightdesc.direction = Vec3(1.f, 0.f, 1.f);

		light->GetLight()->SetLightDesc(lightdesc);

		CUR_SCENE->Add(light);
	}

	//Objs
	{

		//Material
		{
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(_shader);
			auto texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\veigar.jpg");
			material->SetDiffuseMap(texture);
			MaterialDesc& desc = material->GetMaterialDesc();
			desc.ambient = Vec4(1.f);
			desc.diffuse = Vec4(1.f);
			desc.specular = Vec4(1.f);
			RESOURCES->Add(L"Veigar", material);
		}

		//Terrain
		{
			auto obj = make_shared<GameObject>();
			obj->AddComponent(make_shared<Terrain>());
			obj->GetTerrain()->Create(10, 10, RESOURCES->Get<Material>(L"Veigar"));
			CUR_SCENE->Add(obj);
		}

		{
			auto obj = make_shared<GameObject>();
			obj->GetOrAddTransform()->SetPosition(Vec3(0.f));
			obj->AddComponent(make_shared<MeshRenderer>());
			{
				obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Veigar"));
			}
			{
				auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
				obj->GetMeshRenderer()->SetMesh(mesh);
				obj->GetMeshRenderer()->SetPass(0);
			}
			{
				auto collider = make_shared<SphereCollider>();
				collider->SetRadius(0.5f);
				obj->AddComponent(collider);
			}
			CUR_SCENE->Add(obj);
		} 
		{
			auto obj = make_shared<GameObject>();
			obj->GetOrAddTransform()->SetPosition(Vec3(3.f, 0.f, 0.f));
			obj->AddComponent(make_shared<MeshRenderer>());
			{
				obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Veigar"));
			}
			{
				auto mesh = RESOURCES->Get<Mesh>(L"Cube");
				obj->GetMeshRenderer()->SetMesh(mesh);
				obj->GetMeshRenderer()->SetPass(0);
			}
			//{
			//	auto collider = make_shared<AABBBoxCollider>();
			//	collider->GetBoundingBox().Extents = Vec3(0.5f);
			//	obj->AddComponent(collider);
			//}
			{
				obj->GetOrAddTransform()->SetRotation(Vec3(0, 45, 0));

				auto collider = make_shared<OBBBoxCollider>();
				collider->GetBoundingBox().Extents = Vec3(0.5f);
				collider->GetBoundingBox().Orientation = Quaternion::CreateFromYawPitchRoll(45, 0, 0);
				obj->AddComponent(collider);
			}
			{
				//obj->AddComponent(make_shared<MoveScript>());
			}
			CUR_SCENE->Add(obj);
		}
	}
}

void CollisionDemo::Update()
{
	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		int32 mouseX = INPUT->GetMousePos().x;
		int32 mouseY = INPUT->GetMousePos().y;
		
		//PICKING
		auto pickObj = CUR_SCENE->Pick(mouseX, mouseY);
		if (pickObj)
		{
			CUR_SCENE->Remove(pickObj);
		}
	}
}

void CollisionDemo::Render()
{

}
//
//void MoveScript::Update()
//{
//	auto pos = GetTransform()->GetPosition();
//	pos.x -= DT * 1.0f;
//	GetTransform()->SetPosition(pos);
//}
