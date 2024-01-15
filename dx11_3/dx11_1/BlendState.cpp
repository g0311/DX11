#include "pch.h"
#include "BlendState.h"

BlendState::BlendState(ComPtr<ID3D11Device> device)
	: _device(device)
{
}

BlendState::~BlendState()
{
}

void BlendState::Create(D3D11_RENDER_TARGET_BLEND_DESC blendDesc, float factor)
{
	_blendFactor = factor;

	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0] = blendDesc;

	//배경과 텍스쳐를 어떻게 섞는지에 대한 옵션
	//벽 뜷고 보이게 만드는 그런 옵션
	
	/*desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	*/

	HRESULT hr = _device->CreateBlendState(&desc, _blendState.GetAddressOf());
	CHECK(hr);
}