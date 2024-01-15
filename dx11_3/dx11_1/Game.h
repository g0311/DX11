#pragma once

class Game
{
public:
	Game();
	~Game();

public:
	void init(HWND hwnd);
	void Update();
	void Render();

private:
	void CreateGeometry();

private:
	HWND _hwnd;
	shared_ptr<Graphics> _graphics;
	shared_ptr<Pipeline> _pipeline;

private:
	//전체 프로그램 1개 동일 사용 (device,,,)
	//클래스 오크 100마리 같이 사용 (geometry,,,)
	//객체 100마리 중 한마리만 사용 (transformdata,,,)

	//Geometry
	//vector<Vertex> _vertices;
	//ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	//vector<uint32> _indices;
	//ComPtr<ID3D11Buffer> _indexBuffer = nullptr;
	//ComPtr<ID3D11InputLayout> _inputLayout = nullptr;

	shared_ptr<Geometry<VertexTextureData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
	shared_ptr<InputLayout> _inputLayout;

	//VS
	//ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	//ComPtr<ID3DBlob> _vsBlob = nullptr;
	shared_ptr<VertexShader> _vertexShader;

	//RS
	//ComPtr<ID3D11RasterizerState> _rasterizerState = nullptr;
	shared_ptr<RasterizerState> _rasterizerState;

	//PS
	//ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	//ComPtr<ID3DBlob> _psBlob = nullptr;
	shared_ptr<PixelShader> _pixelShader;

	//SRV
	//ComPtr<ID3D11ShaderResourceView> _texture1 = nullptr;
	//ComPtr<ID3D11ShaderResourceView> _texture2 = nullptr;
	shared_ptr<Texture> _texture1;
	shared_ptr<Texture> _texture2;

	//ComPtr<ID3D11SamplerState> _samplerState = nullptr;
	shared_ptr<SamplerState> _samplerState;

	//ComPtr<ID3D11BlendState> _blendState = nullptr;
	shared_ptr<BlendState> _blendState;

	// CPU<->RAM // GPU <-> VRAM

	TransformData _transformData;
	//ComPtr<ID3D11Buffer> _constantBuffer;
	shared_ptr<ConstantBuffer<TransformData>> _constantBuffer;

	Vec3 _localPosition = { 0.f ,0.f ,0.f };
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localScale = { 1.f, 1.f, 1.f };
};

