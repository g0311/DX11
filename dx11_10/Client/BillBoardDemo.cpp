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
#include "TextureBuffer.h"
#include "Viewport.h"
#include "SphereCollider.h"
#include "AABBBoxCollider.h"
#include "OBBBoxCollider.h"
#include "Scene.h"
#include "Terrain.h"
#include "Button.h"
#include "Billboard.h"

#include "BillBoardDemo.h"

void BillBoardDemo::Init()
{
	_shader = make_shared<Shader>(L"28. BillboardDemo.fx");

	//Camera
	{
		auto camera = make_shared<GameObject>();
		camera->GetOrAddTransform()->SetPosition(Vec3{ 0.f, 0.f, -5.f });
		camera->AddComponent(make_shared<Camera>());
		camera->AddComponent(make_shared<CameraScript>());
		camera->GetCamera()->SetCullingMaskLayerOnOff(Layer_UI, true);
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

	// Billboard
	{
		auto obj = make_shared<GameObject>();
		obj->GetOrAddTransform()->SetLocalPosition(Vec3(0.f));
		obj->AddComponent(make_shared<Billboard>());
		{
			// Material
			{
				shared_ptr<Material> material = make_shared<Material>();
				material->SetShader(_shader);
				auto texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\grass.png");
				//auto texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\veigar.jpg");
				material->SetDiffuseMap(texture);
				MaterialDesc& desc = material->GetMaterialDesc();
				desc.ambient = Vec4(1.f);
				desc.diffuse = Vec4(1.f);
				desc.specular = Vec4(1.f);
				RESOURCES->Add(L"Veigar", material);

				obj->GetBillboard()->SetMaterial(material);
			}
		}

		for (int32 i = 0; i < 500; i++)
		{

			Vec2 scale = Vec2(1 + rand() % 3, 1 + rand() % 3);
			Vec2 position = Vec2(-100 + rand() % 200, -100 + rand() % 200);

			obj->GetBillboard()->Add(Vec3(position.x, scale.y * 0.5f, position.y), scale);
		}

		CUR_SCENE->Add(obj);
	}
}

void BillBoardDemo::Update()
{ 

}

void BillBoardDemo::Render()
{

}

void BillBoardTest::Update()
{
	auto go = GetGameObject();

	Vec3 up = Vec3(0, 1, 0);
	Vec3 cameraPos = CUR_SCENE->GetMainCamera()->GetTransform()->GetPosition();
	Vec3 myPos = GetTransform()->GetPosition();

	Vec3 forward = cameraPos - myPos;
	forward.Normalize();

	Matrix lookMatrix = Matrix::CreateWorld(myPos, forward, up);

	Vec3 S, T;
	Quaternion R;
	lookMatrix.Decompose(S, R, T);

	Vec3 rot = Transform::ToEulerAngles(R);

	GetTransform()->SetRotation(rot);
}
