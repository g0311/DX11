#include "pch.h"
#include "RasterizerState.h"

RasterizerState::RasterizerState(ComPtr<ID3D11Device> device)
	: _device(device)
{
}

RasterizerState::~RasterizerState()
{
}

void RasterizerState::Create()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_SOLID; //와이어프레임 가능
	desc.CullMode = D3D11_CULL_BACK; //컬링: 그리지 않게 스킵하는 것//모드는 컬링 기준 설정  
	desc.FrontCounterClockwise = false; //컬링 기준의 기준 설정(시계방향으로 렌더링 됐냐?)

	HRESULT hr = _device->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);
}
