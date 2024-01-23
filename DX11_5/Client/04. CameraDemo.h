#pragma once
#include "IExecute.h"
#include "Geometry.h"

class CameraDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	//Object
	shared_ptr<Shader> _shader;
	shared_ptr<Geometry<VertexColorData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
	Matrix _world = Matrix::Identity;

	//Camera
	shared_ptr<GameObject> _camera;
};

