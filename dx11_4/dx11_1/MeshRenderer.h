#pragma once
#include "Component.h"

class Mesh;
#include "Shader.h"
#include "Material.h"

class MeshRenderer : public Component
{
	using Super = Component;
public:
	MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	virtual ~MeshRenderer();

	void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	shared_ptr<Mesh> GetMesh() { return _mesh; }

	void SetMaterial(shared_ptr<Material> material) { _material = material; }
	shared_ptr<Material> GetMaterial() { return _material; }

	void SetShader(shared_ptr<Shader> shader) { _material->SetShader(shader); }
	shared_ptr<Shader> GetShader() { return _material->GetShader(); }

	void SetTexture(shared_ptr<Texture> texture) { _material->SetTexture(texture); }
	shared_ptr<Texture> GetTexture() { return _material->GetTexture(); }
private:
	ComPtr<ID3D11Device> _device;
	
	friend class RenderManager;
	//Mesh
	shared_ptr<Mesh> _mesh;
	//Material
	shared_ptr<Material> _material;

};

