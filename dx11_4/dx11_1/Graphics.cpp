#include "pch.h"
#include "Graphics.h"

Graphics::Graphics(HWND hwnd)
{
	_hwnd = hwnd;
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();
}

Graphics::~Graphics()
{
}

void Graphics::RenderBegin()
{
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	//�ĸ���ۿ� �׸���� ��û�ϴ� �κ� 	//outputmerge
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	//���� �� �ʱ�ȭ (�� ���̺ѱ�)
	_deviceContext->RSSetViewports(1, &_viewport);
	//����Ʈ ����
}

void Graphics::RenderEnd()
{
	//�ĸ� ���ۿ� �� �׸��� ���⿡ �׸���°���
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}

void Graphics::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc)); //==memset
	{
		desc.BufferDesc.Width = GWinSizeX;
		desc.BufferDesc.Height = GWinSizeY; //���� ũ�⸦ ȭ�� ũ��� �����ִ°� �ո���
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
	CHECK(hr);
}

void Graphics::CreateRenderTargetView()
//�ĸ���ۿ� �׸��� �׸���°� ��� �˷��ٲ���??? ���� Ÿ�� ���°� ���� gpu���� �˷��ٰ�
{
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	HRESULT hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	//�ĸ� ���۸� �ؽ���Ÿ������ ������
	CHECK(hr);

	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	//�װ� ���� Ÿ�� ���°ɷ� �߱޹���(?)
	//gpu�� �׸��� �׸� ���� ������� ������
	CHECK(hr);
}

void Graphics::SetViewport()
{
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(GWinSizeX);
	_viewport.Height = static_cast<float>(GWinSizeY);
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;
}
