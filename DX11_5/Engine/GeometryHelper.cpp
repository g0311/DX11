#include "pch.h"
#include "GeometryHelper.h"

void GeometryHelper::CreateQuad(shared_ptr<Geometry<VertexColorData>> geometry, Color color)
{
	vector<VertexColorData> vertices;
	vertices.resize(4);
	//13
	//02
	vertices[0].position = Vec3{ -0.5f, -0.5f, 0.f };
	vertices[0].color = color;
	vertices[1].position = Vec3{ -0.5f, 0.5f, 0.f };
	vertices[1].color = color;
	vertices[2].position = Vec3{ 0.5f, -0.5f, 0.f };
	vertices[2].color = color;
	vertices[3].position = Vec3{ 0.5f, 0.5f, 0.f };
	vertices[3].color = color;

	geometry->SetVertices(vertices);
	
	vector<uint32> indices = { 0, 1, 2, 2, 1, 3 };
	geometry->SetIndices(indices);
}
