#pragma once
#include "IExecute.h"
#include "Geometry.h"

class MeshDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	// Object
	shared_ptr<GameObject> _obj;
		// Camera
		shared_ptr<GameObject> _camera;
};

