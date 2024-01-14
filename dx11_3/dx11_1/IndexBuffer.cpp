#include "pch.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(ComPtr<ID3D11Device> device)
	: _device(device)
{
}

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::Create(const vector<uint32>& indices)
{
	_stride = sizeof(uint32);
	_count = static_cast<uint32>(indices.size());

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_IMMUTABLE; //cpu<->gpu �۾� ���
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER; //���� �뵵 ����
	desc.ByteWidth = (uint32)_stride * _count; //���� ũ�� ����

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = indices.data();//cpu�� �־��� �޸��� �ּ� ����

	HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());//device�� ���� ���� �� indexbuffer�� �ƿ� ����
	CHECK(hr);
}
