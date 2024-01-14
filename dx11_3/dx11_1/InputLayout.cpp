#include "pch.h"
#include "InputLayout.h"

InputLayout::InputLayout(ComPtr<ID3D11Device> device)
	: _device(device)
{
}

InputLayout::~InputLayout()
{
}

void InputLayout::Create(const vector<D3D11_INPUT_ELEMENT_DESC>& descs, ComPtr<ID3DBlob> blob)
//레이아웃 설명자를 포인터로 받으면 카운트 세기가 애매해짐
{
	//const int32 count = sizeof(descs) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	const int32 count = (uint32)descs.size();
	_device->CreateInputLayout(descs.data(), count, blob->GetBufferPointer(), blob->GetBufferSize(), _inputLayout.GetAddressOf());
	//레이아웃 생성에 셰이더 정보가 필요함
	//인풋 레이아웃이라 vs정보만 필요함
}

