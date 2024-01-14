#pragma once
class VertexBuffer
{
public:
	VertexBuffer(ComPtr<ID3D11Device> device);
	~VertexBuffer();

	ComPtr<ID3D11Buffer> GetComPtr() { return _vertexBuffer; }
	uint32 GetStride() { return _stride; }
	uint32 GetOffset() { return _offset; }
	uint32 GetCount() { return _count; }

	template <typename T>
	void Create(const vector<T> _vertices) 
	{
		_stride = sizeof(T);
		_count = static_cast<uint32>(_vertices.size());

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE; //cpu<->gpu 작업 방법 gpu는 읽기만, cpu는 암것도 못함
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //버퍼 용도 설정
		desc.ByteWidth = (uint32)_stride * _count; //버퍼 크기 설정
		
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data();//cpu에 있었던 메모리의 주소 기입

		HRESULT hr = _device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());//device로 버퍼 생성 후 vertexbuffer에 아웃 저장
		CHECK(hr);
	}
private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11Buffer> _vertexBuffer;
	
	uint32 _stride = 0;
	uint32 _offset = 0;
	uint32 _count = 0;

};