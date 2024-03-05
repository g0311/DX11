#include "pch.h"
#include "Scene.h"

void Scene::Start()
{
	unordered_set<shared_ptr<GameObject>> gameObjects = _gameObjects;
	for (auto obj : gameObjects)
	{
		obj->Start();
	}
}

void Scene::Update()
{
	unordered_set<shared_ptr<GameObject>> gameObjects = _gameObjects;
	for (auto obj : gameObjects)
	{
		obj->Update();
	}
}

void Scene::LateUpdate()
{
	unordered_set<shared_ptr<GameObject>> gameObjects = _gameObjects;
	for (auto obj : gameObjects)
	{
		obj->LateUpdate();
	}

	// INSTANCING
	vector<shared_ptr<GameObject>> temp;
	temp.insert(temp.end(), gameObjects.begin(), gameObjects.end());
	INSTANCING->Render(temp);
}

void Scene::Add(shared_ptr<GameObject> object)
{
	_gameObjects.insert(object);
	
	if (object->GetCamera() != nullptr)
	{
		_cameras.insert(object);
	}

	if (object->GetLight() != nullptr)
	{
		_lights.insert(object);
	}
}

void Scene::Remove(shared_ptr<GameObject> object)
{
	_gameObjects.erase(object);
	_cameras.erase(object);
	_lights.erase(object);
}
