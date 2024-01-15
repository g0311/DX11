#pragma once
#include "Geometry.h"
#include "VertexData.h"

//삼각형 하나 그려준다는가 같은 간단한 도형 그려주는 헬퍼
class GeometryHelper
{
public:
	static void CreateRectangle(std::shared_ptr<Geometry<VertexTextureData>> Geometry);
	static void CreateRectangle(std::shared_ptr<Geometry<VertexColorData>> Geometry, Color color);

private:
};

