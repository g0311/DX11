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
	//후면버퍼에 그리라고 요청하는 부분 	//outputmerge
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	//렌더 색 초기화 (색 들이붇기)
	_deviceContext->RSSetViewports(1, &_viewport);
	//뷰포트 지정
}

void Graphics::RenderEnd()
{
	//후면 버퍼에 다 그리면 여기에 그리라는거임
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}

void Graphics::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc)); //==memset
	{
		desc.BufferDesc.Width = GWinSizeX;
		desc.BufferDesc.Height = GWinSizeY; //버퍼 크기를 화면 크기랑 맞춰주는게 합리적
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
	CHECK(hr);
}

void Graphics::CreateRenderTargetView()
//후면버퍼에 그림을 그리라는걸 어떻게 알려줄꺼냐??? 렌더 타겟 뷰라는걸 만들어서 gpu에게 알려줄것
{
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	HRESULT hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	//후면 버퍼를 텍스쳐타입으로 저장함
	CHECK(hr);

	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	//그걸 렌더 타겟 뷰라는걸로 발급받음(?)
	//gpu가 그림을 그릴 조금 스페셜한 포인터
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
