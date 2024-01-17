#pragma once
#include "Types.h"

//struct Vertex {
//	Vec3 position; //+0 OFFSET
//	//Color color; //+12 OFFSET
//	Vec2 uv;
//};
struct CameraData {
	Matrix matView = Matrix::Identity;
	Matrix matProjection = Matrix::Identity;
};

struct TransformData {
	Matrix matWorld = Matrix::Identity;
};