#include "pch.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{

}

void Game::init(HWND hwnd)
{
	_hwnd = hwnd;
	_width = GWinSizeX;
	_height = GWinSizeY;

	//TODO
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();

	CreateGeometry();
	CreateVS();
	CreateInputLayout();
	CreatePS();

	CreateRasterizerState();
	CreateSamplerState();
	CreateBlendState();

	CreateSRV();
	CreateConstantBuffer();
}

void Game::Update()
{
	_transformData.offset.x = 0.3f;	_transformData.offset.y = 0.3f;
	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));

	_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource); //gpu �Ѳ� ����

	::memcpy(subResource.pData, &_transformData, sizeof(_transformData));

	_deviceContext->Unmap(_constantBuffer.Get(), 0); //gpu �Ѳ� �ݱ�
}

void Game::Render()
{
	RenderBegin();
	
	//TODO
	//IA - VS - RS - PS - OM
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;

		//IA
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		_deviceContext->IASetInputLayout(_inputLayout.Get());
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//VS
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
		_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
		
		//RS �ڵ� �Ұ�. ���� ����
		_deviceContext->RSSetState(_rasterizerState.Get());
		
		//PS
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
		_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView1.GetAddressOf());
		_deviceContext->PSSetShaderResources(1, 1, _shaderResourceView2.GetAddressOf());
		_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());

		//OM
		_deviceContext->OMSetBlendState(_blendState.Get(), nullptr, 0xFFFFFFFF);

		//_deviceContext->Draw(_vertices.size(), 0);
		_deviceContext->DrawIndexed(_indices.size(), 0, 0);
	}

	RenderEnd();
}

void Game::RenderBegin()
{
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	//�ĸ���ۿ� �׸���� ��û�ϴ� �κ� 	//outputmerge
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	//���� �� �ʱ�ȭ (�� ���̺ѱ�)
	_deviceContext->RSSetViewports(1, &_viewport);
	//����Ʈ ����
}

void Game::RenderEnd()
{
	//�ĸ� ���ۿ� �� �׸��� ���⿡ �׸���°���
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}

void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc)); //==memset
	{
		desc.BufferDesc.Width = _width;
		desc.BufferDesc.Height = _height; //���� ũ�⸦ ȭ�� ũ��� �����ִ°� �ո���
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1; //������� ���� ��Ƽ �����ϸ�?
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//���� ���� ����?? //���� ������� ����ҰŴ�!!
		desc.BufferCount = 1;
		desc.OutputWindow = _hwnd; //���� �Ҳ�?
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	//_device.Get();
	//_device.GetAddressOf();

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,//����̹� Ÿ�� gpu���� cpu���� ��ų��?? �̰� gpu
		nullptr,
		0,
		nullptr, //dx ������ �ش��ϴ� ��� ����ؾߵȴ�>> ���� �迭 ����
		0,
		D3D11_SDK_VERSION,
		&desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);
	//�ĸ� ���۰� ���������
	//CHECK(hr);
}

void Game::CreateRenderTargetView()
//�ĸ���ۿ� �׸��� �׸���°� ��� �˷��ٲ���??? ���� Ÿ�� ���°� ���� gpu���� �˷��ٰ�
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	//�ĸ� ���۸� �ؽ���Ÿ������ ������
	CHECK(hr);

	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	//�װ� ���� Ÿ�� ���°ɷ� �߱޹���(?)
	//gpu�� �׸��� �׸� ���� ������� ������
	CHECK(hr);
}

void Game::SetViewport()
{
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;
}

void Game::CreateGeometry()
{
	//vertex data
	{
		_vertices.resize(4);
		//13
		//02
		_vertices[0].position = Vec3(-0.5f, -0.5f, 0);
		_vertices[0].uv = Vec2(0.f, 1.f);
		
		_vertices[1].position = Vec3(-0.5f, 0.5f, 0);
		_vertices[1].uv = Vec2(0.f, 0.f);

		_vertices[2].position = Vec3(0.5f, -0.5f, 0);
		_vertices[2].uv = Vec2(1.f, 1.f);

		_vertices[3].position = Vec3(0.5f, 0.5f, 0);
		_vertices[3].uv = Vec2(1.f, 0.f);
		
		//_vertices[i].color = Color(1.f, 0.f, 0.f, 1.f);
	}
	//vertex buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE; //cpu<->gpu �۾� ��� gpu�� �б⸸, cpu�� �ϰ͵� ����
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //���� �뵵 ����
		desc.ByteWidth = (uint32)sizeof(Vertex) * _vertices.size(); //���� ũ�� ����
		
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data();//cpu�� �־��� �޸��� �ּ� ����

		HRESULT hr = _device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());//device�� ���� ���� �� vertexbuffer�� �ƿ� ����
		CHECK(hr);
	}

	//index data
	{
		_indices = { 0, 1, 2, 2, 1, 3 }; //���� ���� (�ð� or �ݽð�)
		//vertex �ѹ���
	}
	//index buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE; //cpu<->gpu �۾� ���
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER; //���� �뵵 ����
		desc.ByteWidth = (uint32)sizeof(uint32) * _indices.size(); //���� ũ�� ����

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _indices.data();//cpu�� �־��� �޸��� �ּ� ����

		HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());//device�� ���� ���� �� indexbuffer�� �ƿ� ����
		CHECK(hr);
	}
}

void Game::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	_device->CreateInputLayout(layout, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());
	//���̾ƿ� ������ ���̴� ������ �ʿ���
	//��ǲ ���̾ƿ��̶� vs������ �ʿ���
}

void Game::CreateVS()
{//VS�İ�ü �������
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);//vsblob ����

	HRESULT hr = _device->CreateVertexShader(
		_vsBlob->GetBufferPointer(),
		_vsBlob->GetBufferSize(),
		nullptr, 
		_vertexShader.GetAddressOf()
	);
	CHECK(hr);
}

void Game::CreatePS()
{//PS�İ�ü �������
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);//vsblob ����

	HRESULT hr = _device->CreatePixelShader(
		_psBlob->GetBufferPointer(),
		_psBlob->GetBufferSize(),
		nullptr,
		_pixelShader.GetAddressOf()
	);
	CHECK(hr);
}

void Game::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_SOLID; //���̾������� ����
	desc.CullMode = D3D11_CULL_BACK; //�ø�: �׸��� �ʰ� ��ŵ�ϴ� ��//���� �ø� ���� ����  
	desc.FrontCounterClockwise = false; //�ø� ������ ���� ����(�ð�������� ������ �Ƴ�?)

	HRESULT hr = _device->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);
}

void Game::CreateSamplerState()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.BorderColor[0] = 1;
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MaxAnisotropy = 16;
	desc.MaxLOD = FLT_MAX;
	desc.MinLOD = FLT_MIN;
	desc.MipLODBias = 0.0f;

	_device->CreateSamplerState(&desc, _samplerState.GetAddressOf());
}

void Game::CreateBlendState()
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	//���� �ؽ��ĸ� ��� �������� ���� �ɼ�
	//�� ���� ���̰� ����� �׷� �ɼ�
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = _device->CreateBlendState(&desc, _blendState.GetAddressOf());
	CHECK(hr);
}

void Game::CreateSRV()
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;
	HRESULT hr = ::LoadFromWICFile(L"golem.png", WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = ::CreateShaderResourceView(_device.Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView1.GetAddressOf());
	CHECK(hr);

	hr = ::LoadFromWICFile(L"skeleton.png", WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = ::CreateShaderResourceView(_device.Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView2.GetAddressOf());
	CHECK(hr);

}

void Game::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC; // cpu : write, gtpu : read
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(TransformData);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = _device->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
	CHECK(hr);
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		blob.GetAddressOf(),
		nullptr
	);
}

 
