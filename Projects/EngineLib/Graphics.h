#pragma once
class Graphics
{
private:
	static Graphics* _instance;
public:
	static Graphics* GetInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new Graphics();
		}

		return _instance;
	}
private:
	Graphics();
	~Graphics();
private:
	HWND		_hwnd;
	uint32		_width = 0;
	uint32		_height = 0;
private:
	//Device SwapChain
	ComPtr<ID3D11Device>		_device;
	ComPtr<ID3D11DeviceContext>	_deviceContext;
	ComPtr<IDXGISwapChain>		_swapChain;
	//RTV
	ComPtr<ID3D11Texture2D> _backBuffer;
	ComPtr<ID3D11RenderTargetView>	_renderTargetView[2];
	//DSV
	ComPtr<ID3D11Texture2D> _depthStancilTexture;
	ComPtr<ID3D11DepthStencilView> _depthStancilView;
	//Misc
	D3D11_VIEWPORT				_viewPort = { 0, };
	float						_clearColor[4] = { 0.f,0.f,0.f,0.f };
	float						_clearColor2[4] = { 0.5f,0.5f,0.5f,1.f };
public:
	ComPtr<ID3D11Device>		GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }
	ComPtr<IDXGISwapChain>		GetSwapChain() { return _swapChain; }
	ComPtr<ID3D11RenderTargetView> GetRenderTargetView(UINT16 num) { if(num < 8) return _renderTargetView[num]; }
	ComPtr<ID3D11Texture2D> GetRenderTexture() { return _backBuffer; }
private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void CreateDepthStencilView();
	void SetViewPort();
public:
	void Init();
	void RenderBegin();
	void RenderEnd();
};

