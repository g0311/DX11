#include "pch.h"
#include "15. EmissiveDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"
#include "MeshRenderer.h"

void EmissiveDemo::Init()
{
	RESOURCES->Init();

	//Shader
	_shader = make_shared<Shader>(L"12. Lighting_Emissive.fx");

	//Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform()->SetPosition(Vec3(0.f, 0.f, -10.f));
	_camera->GetTransform()->SetRotation(Vec3(0, 20, 0));
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	//Object
	_obj = make_shared<GameObject>();
	_obj->GetOrAddTransform();
	_obj->AddComponent(make_shared<MeshRenderer>());
	{
		_obj->GetMeshRenderer()->SetShader(_shader);
	}
	{
		auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
		_obj->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		auto texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\veigar.jpg");
		_obj->GetMeshRenderer()->SetTexture(texture);
	}

	//Object2
	_obj2 = make_shared<GameObject>();
	_obj2->GetOrAddTransform()->SetPosition(Vec3(0.5f, 0.f, 2.f));
	_obj2->AddComponent(make_shared<MeshRenderer>());
	{
		_obj2->GetMeshRenderer()->SetShader(_shader);
	}
	{
		auto mesh = RESOURCES->Get<Mesh>(L"Cube");
		_obj2->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		auto texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\veigar.jpg");
		_obj2->GetMeshRenderer()->SetTexture(texture);
	}

	RENDER->Init(_shader);//상수 버퍼 생성
}

void EmissiveDemo::Update()
{
	_camera->Update();

	RENDER->Update(); //상수 버퍼 업데이트

	//각 오브젝트 색상 지정
	//원랜 각 오브젝트가 가지는 머티리얼에 지정 >> 이건 메시 렌더러가 가지는 경우가 많음
	{
		Vec4 materialAmbient(1.f, 0.f, 0.f, 1.f);
		_shader->GetVector("MaterialEmissive")->SetFloatVector((float*)&materialAmbient);
		_obj->Update();
	}
	{
		Vec4 materialAmbient(1.f);
		_shader->GetVector("MaterialEmissive")->SetFloatVector((float*)&materialAmbient);
		_obj2->Update();
	}
}

void EmissiveDemo::Render()
{
}
