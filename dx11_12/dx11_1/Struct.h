#pragma once
#include "Types.h"

struct Vertex {
	Vec3 position; //+0 OFFSET
	//Color color; //+12 OFFSET
	Vec2 uv;
};

struct TransformData {
	Vec3 offset; //12by
	float dummy; //4by (��� ���۴� 16����Ʈ ������ ���־���ϹǷ� �����Ⱚ ����)
};