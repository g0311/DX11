#include "pch.h"
#include "MeshRenderer.h"
#include "Game.h"
#include "Mesh.h"

MeshRenderer::MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	: Super(ComponentType::MeshRenderer)
{
}

MeshRenderer::~MeshRenderer()
{
}
