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

	_graphics = make_shared<Graphics>(hwnd);
	//com객체랑 비슷한듯
	_vertexBuffer = make_shared<VertexBuffer>(_graphics->GetDevice());
	_indexBuffer = make_shared<IndexBuffer>(_graphics->GetDevice());
	_inputLayout = make_shared<InputLayout>(_graphics->GetDevice());

	//TODO
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
	//_transformData.offset.x = 0.3f;	_transformData.offset.y = 0.3f;
	_localPosition.x += 0.0001f;

	Matrix matScale = Matrix::CreateScale(_localScale / 3);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	Matrix matWorld = matScale * matRotation * matTranslation;
	_transformData.matWorld = matWorld;

	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));

	
	_graphics->GetDeviceContext()->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource); //gpu 뚜껑 열기

	::memcpy(subResource.pData, &_transformData, sizeof(_transformData));

	_graphics->GetDeviceContext()->Unmap(_constantBuffer.Get(), 0); //gpu 뚜껑 닫기
}

void Game::Render()
{
	_graphics->RenderBegin();
	
	//TODO
	//IA - VS - RS - PS - OM
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;

		auto _deviceContext = _graphics->GetDeviceContext();
		//IA
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
		_deviceContext->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
		_deviceContext->IASetInputLayout(_inputLayout->GetComPtr().Get());
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

	_graphics->RenderEnd();
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
		//vertexes local position..
		//_vertices[i].color = Color(1.f, 0.f, 0.f, 1.f);
	}
	//vertex buffer
	{
		_vertexBuffer->Create(_vertices);
	}

	//index data
	{
		_indices = { 0, 1, 2, 2, 1, 3 }; //방향 유지 (시계 or 반시계)
		//vertex 넘버링
	}
	//index buffer
	{
		_indexBuffer->Create(_indices);
	}
}

void Game::CreateInputLayout()
{
	vector<D3D11_INPUT_ELEMENT_DESC> layout =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	_inputLayout->Create(layout, _vsBlob);
}

void Game::CreateVS()
{//VS컴객체 만들어짐
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);//vsblob 저장

	HRESULT hr = _graphics->GetDevice()->CreateVertexShader(
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

	HRESULT hr = _graphics->GetDevice()->CreatePixelShader(
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

	HRESULT hr = _graphics->GetDevice()->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
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

	_graphics->GetDevice()->CreateSamplerState(&desc, _samplerState.GetAddressOf());
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

	HRESULT hr = _graphics->GetDevice()->CreateBlendState(&desc, _blendState.GetAddressOf());
	CHECK(hr);
}

void Game::CreateSRV()
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;
	HRESULT hr = ::LoadFromWICFile(L"golem.png", WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = ::CreateShaderResourceView(_graphics->GetDevice().Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView1.GetAddressOf());
	CHECK(hr);

	hr = ::LoadFromWICFile(L"skeleton.png", WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = ::CreateShaderResourceView(_graphics->GetDevice().Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView2.GetAddressOf());
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

	HRESULT hr = _graphics->GetDevice()->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
	CHECK(hr);
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	HRESULT hr = ::D3DCompileFromFile(
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
	CHECK(hr);
}