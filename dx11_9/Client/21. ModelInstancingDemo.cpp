#include "pch.h"
#include "21. ModelInstancingDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"

void ModelInstancingDemo::Init()
{
	RESOURCES->Init();

	_shader = make_shared<Shader>(L"21. ModelInstancingDemo.fx");

	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform()->SetPosition(Vec3{ 0.f, 0.f, -5.f });
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	shared_ptr<class Model> m1 = make_shared<Model>();
	m1->ReadModel(L"Tower/Tower");
	m1->ReadMaterial(L"Tower/Tower");

	for (int32 i = 0; i < 500; i++)
	{
		auto obj = make_shared<GameObject>();
		obj->GetOrAddTransform()->SetPosition(Vec3{ (float)(rand() % 100), 0, (float)(rand() % 100) });
		obj->GetOrAddTransform()->SetScale(Vec3(0.01f));
		obj->AddComponent(make_shared<ModelRenderer>(_shader));
		{
			obj->GetModelRenderer()->SetModel(m1);
		}
		_objs.push_back(obj);
	}

	RENDER->Init(_shader);
}

void ModelInstancingDemo::Update()
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

	INSTANCING->Render(_objs);
}

void ModelInstancingDemo::Render()
{

}