#include "pch.h"
#include "Material.h"

Material::Material()
	: Super(ResourceType::Material)
{
}

Material::~Material()
{
}

void Material::SetShader(shared_ptr<Shader> shader)
{
	_shader = shader;

	_diffuseEffectBuffer = shader->GetSRV("DiffuseMap");
	_normalEffectBuffer = shader->GetSRV("NormalMap");
	_specularEffectBuffer = shader->GetSRV("SpecularMap");
	//얘넨 상수버퍼가 아니라 텍스쳐임!!!
}

void Material::Update()
{
	if (_shader == nullptr)
		return;

	//머티리얼 버퍼 지정 (인자 값 삽입) //머티리얼 빛 합성 색
	RENDER->PushMaterialData(_desc);

	//텍스쳐 설정 //머티리얼 각각마다 값이 존재
	if (_diffuseMap)
		_diffuseEffectBuffer->SetResource(_diffuseMap->GetComPtr().Get());
	if (_normalMap)
		_normalEffectBuffer->SetResource(_normalMap->GetComPtr().Get());
	if (_specularMap)
		_specularEffectBuffer->SetResource(_specularMap->GetComPtr().Get());
}

shared_ptr<Material> Material::Clone()
{
	shared_ptr<Material> material = make_shared<Material>();

	material->_desc = _desc; //얘만 스트럭트라 복사 생성 나머지는 공유 포인터
	material->_shader = _shader;
	material->_diffuseMap = _diffuseMap;
	material->_normalMap = _normalMap;
	material->_specularMap = _specularMap;
	material->_diffuseEffectBuffer = _diffuseEffectBuffer;
	material->_normalEffectBuffer = _normalEffectBuffer;
	material->_specularEffectBuffer = _specularEffectBuffer;

	return material;
}
