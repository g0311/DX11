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

	//com객체랑 비슷한듯
	_graphics = make_shared<Graphics>(hwnd);

	_geometry = make_shared<Geometry<VertexTextureData>>();

	_vertexBuffer = make_shared<VertexBuffer>(_graphics->GetDevice());
	_indexBuffer = make_shared<IndexBuffer>(_graphics->GetDevice());
	_inputLayout = make_shared<InputLayout>(_graphics->GetDevice());

	_vertexShader = make_shared<VertexShader>(_graphics->GetDevice());
	_pixelShader = make_shared<PixelShader>(_graphics->GetDevice());

	_constantBuffer = make_shared<ConstantBuffer<TransformData>>(_graphics->GetDevice(), _graphics->GetDeviceContext());

	_texture1 = make_shared<Texture>(_graphics->GetDevice());
	_texture2 = make_shared<Texture>(_graphics->GetDevice());

	_rasterizerState = make_shared<RasterizerState>(_graphics->GetDevice());
	_samplerState = make_shared<SamplerState>(_graphics->GetDevice());
	_blendState = make_shared<BlendState>(_graphics->GetDevice());

	_pipeline = make_shared<Pipeline>(_graphics->GetDeviceContext());

	//TODO
	CreateGeometry();
	//CreateVS();
	_vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");
	//CreateInputLayout();
	_inputLayout->Create(VertexTextureData::descs, _vertexShader->GetBlob());
	//CreatePS();
	_pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");

	//CreateRasterizerState();
	_rasterizerState->Create();
	//CreateSamplerState();
	_samplerState->Create();
	//CreateBlendState();
	_blendState->Create();

	_constantBuffer->Create();

	_texture1->Create(L"golem.png");
	_texture2->Create(L"skeleton.png");
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

	_constantBuffer->CopyData(_transformData);
}

void Game::Render()
{
	_graphics->RenderBegin();
	
	//TODO
	//IA - VS - RS - PS - OM
	{
		PipelineInfo info;
		info.inputLayout = _inputLayout;
		info.vertexShader = _vertexShader;
		info.pixelShader = _pixelShader;
		info.rasterizerState = _rasterizerState;
		info.blendState = _blendState;

		_pipeline->UpdatePipeline(info);

		uint32 stride = sizeof(VertexTextureData);
		uint32 offset = 0;

		_pipeline->SetVertexBuffer(_vertexBuffer);
		_pipeline->SetIndexBuffer(_indexBuffer);

		_pipeline->SetConstantBuffer<TransformData>(0, SS_VertexShader, _constantBuffer);

		_pipeline->SetTexture(0, SS_PixelShader, _texture1);
		_pipeline->SetTexture(0, SS_PixelShader, _texture2);

		_pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);
		
		_pipeline->DrawIndexed(_geometry->GetIndices().size(), 0, 0);
	}//무조건 파이프라인 순서를 지켜야 할 필요는 없다..? 컴퓨터가 알아서 수행? >> dx가 명령어 스케쥴링 해준대

	_graphics->RenderEnd();
}

void Game::CreateGeometry()
{
	//vertex data
	{
		GeometryHelper::CreateRectangle(_geometry);
	}
	//vertex buffer
	{
		_vertexBuffer->Create(_geometry->GetVertices());
	}
	//index buffer
	{
		_indexBuffer->Create(_geometry->GetIndices());
	}
}
