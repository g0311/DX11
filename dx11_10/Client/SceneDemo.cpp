#include "pch.h"
#include "SceneDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"
#include "Light.h"

void SceneDemo::Init()
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
		{
			shared_ptr<class Model> m1 = make_shared<Model>();
			m1->ReadModel(L"Kachujin/Kachujin");
			m1->ReadMaterial(L"Kachujin/Kachujin");
			m1->ReadAnimation(L"Kachujin/Idle");
			m1->ReadAnimation(L"Kachujin/Run");
			m1->ReadAnimation(L"Kachujin/Slash");

			for (int32 i = 0; i < 500; i++)
			{
				auto obj = make_shared<GameObject>();
				obj->GetOrAddTransform()->SetPosition(Vec3{ (float)(rand() % 100), 0, (float)(rand() % 100) });
				obj->GetOrAddTransform()->SetScale(Vec3(0.01f));
				obj->AddComponent(make_shared<ModelAnimator>(_shader));
				{
					obj->GetModelAnimator()->SetModel(m1);
					obj->GetModelAnimator()->SetPass(2);
				}
				CUR_SCENE->Add(obj);
			}
		}
		{
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
			}

			for (int32 i = 0; i < 100; i++)
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
					obj->GetMeshRenderer()->SetPass(0);
				}
				CUR_SCENE->Add(obj);
			}
		}
		{
			shared_ptr<class Model> m1 = make_shared<Model>();
			m1->ReadModel(L"Tower/Tower");
			m1->ReadMaterial(L"Tower/Tower");

			for (int32 i = 0; i < 10; i++)
			{
				auto obj = make_shared<GameObject>();
				obj->GetOrAddTransform()->SetPosition(Vec3{ (float)(rand() % 100), 0, (float)(rand() % 100) });
				obj->GetOrAddTransform()->SetScale(Vec3(0.01f));
				obj->AddComponent(make_shared<ModelRenderer>(_shader));
				{
					obj->GetModelRenderer()->SetModel(m1);
					obj->GetModelRenderer()->SetPass(1);
				}
				CUR_SCENE->Add(obj);
			}
		}
	}
}

void SceneDemo::Update()
{
}

void SceneDemo::Render()
{

}
