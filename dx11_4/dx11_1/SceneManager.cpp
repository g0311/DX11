#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Game.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Animator.h"

SceneManager::SceneManager(shared_ptr<Graphics> graphics)
	: _graphics(graphics)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Update();
	_activeScene->FixedUpdate();
	_activeScene->LateUpdate();
}

void SceneManager::LoadScene(wstring SceneName)
{
	//원래 Resources 찾아서 _activeScene = ~~
	_activeScene = LoadTestScene();
	Init();
}

shared_ptr<Scene> SceneManager::LoadTestScene()
{
	shared_ptr<Scene> scene = make_shared<Scene>();

	//TODO
	
	//GO

	//Monster
	{
		shared_ptr<GameObject> monster = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
		{
			monster->GetOrAddTransform();

			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext());
			monster->AddComponent(meshRenderer);

			shared_ptr<Mesh> mesh = RESOURCES->Get<Mesh>(L"Rectangle");
			meshRenderer->SetMesh(mesh);

			shared_ptr<Material> material = RESOURCES->Get<Material>(L"Default");
			meshRenderer->SetMaterial(material);
		}
		{
			shared_ptr<Animator> animator = make_shared<Animator>();
			shared_ptr<Animation> animation = RESOURCES->Get<Animation>(L"SnakeAnim");
			animator->SetAnimation(animation);
			monster->AddComponent(animator);
		}
		//_monster->GetTransform()->SetScale(Vec3(100, 100, 100));
		scene->AddGameObject(monster);
	}

	//Monster
	{
		shared_ptr<GameObject> monster = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
		{
			monster->GetOrAddTransform()->SetPosition(Vec3{ 1.f,1.f,0.f });

			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext());
			monster->AddComponent(meshRenderer);

			shared_ptr<Mesh> mesh = RESOURCES->Get<Mesh>(L"Rectangle");
			meshRenderer->SetMesh(mesh);

			shared_ptr<Material> material = RESOURCES->Get<Material>(L"Default");
			meshRenderer->SetMaterial(material);
		}
		{
			shared_ptr<Animator> animator = make_shared<Animator>();
			shared_ptr<Animation> animation = RESOURCES->Get<Animation>(L"SnakeAnim");
			animator->SetAnimation(animation);
			monster->AddComponent(animator);
		}
		//_monster->GetTransform()->SetScale(Vec3(100, 100, 100));
		scene->AddGameObject(monster);
	}

	//Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
		camera->GetOrAddTransform();
		camera->AddComponent(make_shared<Camera>());
		scene->AddGameObject(camera);
	}

	return scene;
}
