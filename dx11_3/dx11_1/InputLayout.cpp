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
//���̾ƿ� �����ڸ� �����ͷ� ������ ī��Ʈ ���Ⱑ �ָ�����
{
	//const int32 count = sizeof(descs) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	const int32 count = (uint32)descs.size();
	_device->CreateInputLayout(descs.data(), count, blob->GetBufferPointer(), blob->GetBufferSize(), _inputLayout.GetAddressOf());
	//���̾ƿ� ������ ���̴� ������ �ʿ���
	//��ǲ ���̾ƿ��̶� vs������ �ʿ���
}

