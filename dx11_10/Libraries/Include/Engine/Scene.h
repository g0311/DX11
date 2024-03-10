#pragma once

class Scene
{
public:
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();

	virtual void Add(shared_ptr<GameObject> object);
	virtual void Remove(shared_ptr<GameObject> object);

	unordered_set<shared_ptr<GameObject>>& GetObjects() { return _gameObjects; }
	shared_ptr<GameObject> GetMainCamera();
	shared_ptr<GameObject> GetUICamera();
	shared_ptr<GameObject> GetLight() { return _lights.empty() ? nullptr : *_lights.begin(); }

	void PickUI();
	shared_ptr<GameObject> Pick(int32 screenX, int32 screenY);

	void CheckCollision();

private:
	unordered_set<shared_ptr<GameObject>> _gameObjects;
	//cache camera
	unordered_set<shared_ptr<GameObject>> _cameras;
	//cache light
	unordered_set<shared_ptr<GameObject>> _lights;
};
