#pragma once

class Scene
{
public:
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	virtual void Add(shared_ptr<GameObject> object);
	virtual void Remove(shared_ptr<GameObject> object);

private:
	unordered_set<shared_ptr<GameObject>> _gameObjects;
	//cache camera
	unordered_set<shared_ptr<GameObject>> _cameras;
	//cache light
	unordered_set<shared_ptr<GameObject>> _lights;
};

