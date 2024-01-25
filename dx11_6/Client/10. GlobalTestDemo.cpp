#include "pch.h"
#include "10. GlobalTestDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"
#include "MeshRenderer.h"

void GlobalTestDemo::Init()
{
	//Shader
	_shader = make_shared<Shader>(L"08. GlobalTest.fx");

	//Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	_camera->GetTransform()->SetPosition(Vec3(0.f, 0.5f, -2.f));

	_obj = make_shared<GameObject>();
	_obj->GetOrAddTransform();
	_obj->AddComponent(make_shared<MeshRenderer>());
	{
		_obj->GetMeshRenderer()->SetShader(_shader);
	}
	{
		RESOURCES->Init();
		auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
		_obj->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		auto texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\veigar.jpg");
		_obj->GetMeshRenderer()->SetTexture(texture);
	}

	RENDER->Init(_shader);//상수 버퍼 생성
}

void GlobalTestDemo::Update()
{
	_camera->Update();

	RENDER->Update(); //상수 버퍼 업데이트

	_obj->Update();
}

void GlobalTestDemo::Render()
{
}
