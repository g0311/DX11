#include "pch.h"
#include "Shader.h"

Shader::Shader(ComPtr<ID3D11Device> device)
{
	_device = device;
}

Shader::~Shader()
{
}

void Shader::Create(const wstring& path, const string& name, const string& version)
{
}

void Shader::LoadShaderFromFile(const wstring& path, const string& name, const string& version)
{
	_path = path;
	_name = name;
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		Blob.GetAddressOf(),
		nullptr
	);
	CHECK(hr);
}

VertexShader::VertexShader(ComPtr<ID3D11Device> device) : Super(device)
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::Create(const wstring& path, const string& name, const string& version)
{
	LoadShaderFromFile(path, name, version);
	HRESULT hr = _device->CreateVertexShader(
		Blob->GetBufferPointer(),
		Blob->GetBufferSize(),
		nullptr,
		_vertexShader.GetAddressOf()
	);
	CHECK(hr);
}

PixelShader::PixelShader(ComPtr<ID3D11Device> device) : Super(device)
{
}

PixelShader::~PixelShader()
{
}

void PixelShader::Create(const wstring& path, const string& name, const string& version)
{
	LoadShaderFromFile(path, name, version);
	HRESULT hr = _device->CreatePixelShader(
		Blob->GetBufferPointer(),
		Blob->GetBufferSize(),
		nullptr,
		_pixelShader.GetAddressOf()
	);
	CHECK(hr);
}
