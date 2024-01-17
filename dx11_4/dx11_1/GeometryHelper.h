#pragma once
#include "Geometry.h"
#include "VertexData.h"

//�ﰢ�� �ϳ� �׷��شٴ°� ���� ������ ���� �׷��ִ� ����
class GeometryHelper
{
public:
	static void CreateRectangle(std::shared_ptr<Geometry<VertexTextureData>> Geometry);
	static void CreateRectangle(std::shared_ptr<Geometry<VertexColorData>> Geometry, Color color);

private:
};

