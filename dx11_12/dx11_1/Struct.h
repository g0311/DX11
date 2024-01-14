#pragma once
#include "Types.h"

struct Vertex {
	Vec3 position; //+0 OFFSET
	//Color color; //+12 OFFSET
	Vec2 uv;
};

struct TransformData {
	Vec3 offset; //12by
	float dummy; //4by (상수 버퍼는 16바이트 정렬을 해주어야하므로 쓰레기값 지정)
};