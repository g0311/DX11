#pragma once

class Graphics
{//공용 사용 부분 담을 클래스 //디바이스, 컨텍스트, 스왑체인 등
public:
	Graphics(HWND hwnd);
	~Graphics();

	void RenderBegin();
	void RenderEnd();

	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();

private:
	HWND _hwnd;

private:
	//DX
	ComPtr<ID3D11Device> _device = nullptr; //생성
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr; //오더
	ComPtr<IDXGISwapChain> _swapChain = nullptr; //더블버퍼
	//이 세놈은 중요함...

	//RT
	ComPtr<ID3D11RenderTargetView> _renderTargetView = nullptr; //후면 버퍼 렌더링 요청 할 곳

	//MISC
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
};

