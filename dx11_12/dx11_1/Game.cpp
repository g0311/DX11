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

	_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource); //gpu 뚜껑 열기

	::memcpy(subResource.pData, &_transformData, sizeof(_transformData));

	_deviceContext->Unmap(_constantBuffer.Get(), 0); //gpu 뚜껑 닫기
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
		
		//RS 코딩 불가. 설정 가능
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
	//후면버퍼에 그리라고 요청하는 부분 	//outputmerge
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	//렌더 색 초기화 (색 들이붇기)
	_deviceContext->RSSetViewports(1, &_viewport);
	//뷰포트 지정
}

void Game::RenderEnd()
{
	//후면 버퍼에 다 그리면 여기에 그리라는거임
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}

void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc)); //==memset
	{
		desc.BufferDesc.Width = _width;
		desc.BufferDesc.Height = _height; //버퍼 크기를 화면 크기랑 맞춰주는게 합리적
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1; //계단현상 방지 멀티 스케일링?
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//버퍼 뭐로 쓸거?? //최종 출력으로 사용할거다!!
		desc.BufferCount = 1;
		desc.OutputWindow = _hwnd; //뭘로 할꺼?
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	//_device.Get();
	//_device.GetAddressOf();

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,//드라이버 타입 gpu쓸래 cpu한테 시킬래?? 이건 gpu
		nullptr,
		0,
		nullptr, //dx 버전에 해당하는 기능 사용해야된다>> 버전 배열 전달
		0,
		D3D11_SDK_VERSION,
		&desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);
	//후면 버퍼가 만들어졌땅
	//CHECK(hr);
}

void Game::CreateRenderTargetView()
//후면버퍼에 그림을 그리라는걸 어떻게 알려줄꺼냐??? 렌더 타겟 뷰라는걸 만들어서 gpu에게 알려줄것
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	//후면 버퍼를 텍스쳐타입으로 저장함
	CHECK(hr);

	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	//그걸 렌더 타겟 뷰라는걸로 발급받음(?)
	//gpu가 그림을 그릴 조금 스페셜한 포인터
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
		desc.Usage = D3D11_USAGE_IMMUTABLE; //cpu<->gpu 작업 방법 gpu는 읽기만, cpu는 암것도 못함
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //버퍼 용도 설정
		desc.ByteWidth = (uint32)sizeof(Vertex) * _vertices.size(); //버퍼 크기 설정
		
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data();//cpu에 있었던 메모리의 주소 기입

		HRESULT hr = _device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());//device로 버퍼 생성 후 vertexbuffer에 아웃 저장
		CHECK(hr);
	}

	//index data
	{
		_indices = { 0, 1, 2, 2, 1, 3 }; //방향 유지 (시계 or 반시계)
		//vertex 넘버링
	}
	//index buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE; //cpu<->gpu 작업 방법
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER; //버퍼 용도 설정
		desc.ByteWidth = (uint32)sizeof(uint32) * _indices.size(); //버퍼 크기 설정

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _indices.data();//cpu에 있었던 메모리의 주소 기입

		HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());//device로 버퍼 생성 후 indexbuffer에 아웃 저장
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
	//레이아웃 생성에 셰이더 정보가 필요함
	//인풋 레이아웃이라 vs정보만 필요함
}

void Game::CreateVS()
{//VS컴객체 만들어짐
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);//vsblob 저장

	HRESULT hr = _device->CreateVertexShader(
		_vsBlob->GetBufferPointer(),
		_vsBlob->GetBufferSize(),
		nullptr, 
		_vertexShader.GetAddressOf()
	);
	CHECK(hr);
}

void Game::CreatePS()
{//PS컴객체 만들어짐
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);//vsblob 저장

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
	desc.FillMode = D3D11_FILL_SOLID; //와이어프레임 가능
	desc.CullMode = D3D11_CULL_BACK; //컬링: 그리지 않게 스킵하는 것//모드는 컬링 기준 설정  
	desc.FrontCounterClockwise = false; //컬링 기준의 기준 설정(시계방향으로 렌더링 됐냐?)

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

	//배경과 텍스쳐를 어떻게 섞는지에 대한 옵션
	//벽 뜷고 보이게 만드는 그런 옵션
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

 
