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
	desc.Usage = D3D11_USAGE_IMMUTABLE; //cpu<->gpu 작업 방법
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER; //버퍼 용도 설정
	desc.ByteWidth = (uint32)_stride * _count; //버퍼 크기 설정

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = indices.data();//cpu에 있었던 메모리의 주소 기입

	HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());//device로 버퍼 생성 후 indexbuffer에 아웃 저장
	CHECK(hr);
}
