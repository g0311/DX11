#pragma once

class Graphics
{//���� ��� �κ� ���� Ŭ���� //����̽�, ���ؽ�Ʈ, ����ü�� ��
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
	ComPtr<ID3D11Device> _device = nullptr; //����
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr; //����
	ComPtr<IDXGISwapChain> _swapChain = nullptr; //�������
	//�� ������ �߿���...

	//RT
	ComPtr<ID3D11RenderTargetView> _renderTargetView = nullptr; //�ĸ� ���� ������ ��û �� ��

	//MISC
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
};

