#include "Window.h"
#include "EngineException.h"
#include "Buffers.h"
#include "Input.h"

#include "Resources.h"

#include <stack>
#include <CommCtrl.h>

std::stack<MENUITEM> gMenuStack;

const FLOAT gClearColor[] = RGBA_UNORM(0, 0, 0, 0); //RGBA_UNORM(135, 206, 235, 255);
LRESULT(CALLBACK* Window::CustomWindowProc)(HWND, UINT, WPARAM, LPARAM);

Window::Window(HINSTANCE instance, LPCWSTR className, LPCWSTR wndName, int width, int height, int vpWidth, int vpHeight, int nCmdShow) : 
	mInstance(instance), mClassName(className), mWndName(wndName), mWidth(width), mHeight(height), mViewportWidth(vpWidth), mViewportHeight(vpHeight),
	mCmdShow(nCmdShow), mWnd(NULL), mWndViewport(NULL) {
	Initialize();
}

int Window::GetWidth() const {
	return mWidth;
}

int Window::GetHeight() const {
	return mHeight;
}

int Window::GetViewportWidth() const {
	return mViewportWidth;
}

int Window::GetViewportHeight() const {
	return mViewportHeight;
}

HINSTANCE Window::GetInstance() const {
	return mInstance;
}

HWND Window::GetWnd() const {
	return mWnd;
}

void Window::Close() {
	PostQuitMessage(0);
}

void Window::ExtendWndProc(LRESULT(CALLBACK* func)(HWND, UINT, WPARAM, LPARAM)) {
	CustomWindowProc = func;
}

HMENU Window::SubMenuBegin(LPCWSTR lpNameItem, UINT uType) {
	MENUITEM mItem;
	mItem.hMenu = CreateMenu();
	mItem.uFlag = uType;
	mItem.lpItemName = lpNameItem;

	gMenuStack.push(mItem);
	return mItem.hMenu;
}

HMENU Window::MenuBegin() {
	return SubMenuBegin(L"", 0);
}

BOOL Window::SubMenuEnd() {
	MENUITEM mItem = gMenuStack.top();
	gMenuStack.pop();
	return AddItem((UINT_PTR)mItem.hMenu, mItem.lpItemName, mItem.uFlag);
}

BOOL Window::MenuEnd() {
	MENUITEM mItem = gMenuStack.top();
	gMenuStack.pop();
	return SetMenu(mWnd, mItem.hMenu);
}

BOOL Window::AddItem(UINT_PTR uIDNewItem, LPCWSTR lpNameItem, UINT uType) {
	return AppendMenu(gMenuStack.top().hMenu, uType, uIDNewItem, lpNameItem);
}

LRESULT CALLBACK Window::CustomDefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (CustomWindowProc == NULL) {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return (*CustomWindowProc)(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_MOUSEWHEEL:
			Input::mouseScrollDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			return 0;
	}
	return CustomDefWindowProc(hWnd, message, wParam, lParam);
}

HWND Window::CreateControl(DWORD dwICC, LPCWSTR lpClassName, int x, int y, int width, int height) {
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = dwICC;
	InitCommonControlsEx(&icex);

	HWND control;
	if ((control = CreateWindow(lpClassName, L"", WS_CHILD | WS_VISIBLE, x, y, width, height, mWnd, NULL, mInstance, NULL)) == NULL) {
		throw EngineException(L"CreateControl() failed.");
	}
	return control;
}

void Window::Initialize() {
	InitializeWindow();	// WINAPI initialization
	InitializeDirectX(); // DirectX initialization
}

void Window::InitializeWindow() {
	// Describe window class
	WNDCLASSEX winClass;
	ZeroMemory(&winClass, sizeof(WNDCLASSEX));
	winClass.cbSize			= sizeof(WNDCLASSEX);
	winClass.lpfnWndProc	= WndProc;
	winClass.hInstance		= mInstance;
	winClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winClass.lpszClassName	= mClassName;
	winClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	winClass.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);
	winClass.hbrBackground	= GetSysColorBrush(COLOR_BTNFACE);
	winClass.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDR_MAINMENU);
	winClass.style			= CS_HREDRAW | CS_VREDRAW;

	// Make a registration of window class for OS
	if (RegisterClassEx(&winClass) == 0) {
		throw EngineException(L"RegisterClassEx() failed.");
	}
	// Create window
	if ((mWnd = CreateWindow(mClassName, mWndName, WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_THICKFRAME), 0, 0, mWidth, mHeight, NULL, NULL, mInstance, NULL)) == NULL) {
		throw EngineException(L"CreateWindow() failed.");
	}
	if ((mWndViewport = CreateWindow(WC_LISTBOX, L"", WS_CHILD | WS_VISIBLE, 0, 0, mViewportWidth, mViewportHeight, mWnd, NULL, mInstance, NULL)) == NULL) {
		throw EngineException(L"CreateWindow() failed.");
	}

	ShowWindow(mWnd, mCmdShow);
	UpdateWindow(mWnd);
}

void Window::InitializeDirectX() {
	HRESULT hr = 0;

	UINT flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	const D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	// Create Diretc3D device and context device
	// Direct3D device needs for initialization other features
	// Direct3D context device needs for rendering pipeline management
	if (FAILED(hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &mDevice, &mLevel, &mContext))) {
		throw EngineException(L"D3D11CreateDevice() failed.", hr);
	}

	// Create swap chain
	ComPtr<IDXGIDevice> dxgiDevice;
	ComPtr<IDXGIAdapter> dxgiAdapter;
	ComPtr<IDXGIFactory> dxgiFactory;

	if (FAILED(hr = mDevice->QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice))) {
		throw EngineException(L"ID3DDevice::QueryInterface() failed.", hr);
	}
	if (FAILED(hr = dxgiDevice->GetAdapter(&dxgiAdapter))) {
		throw EngineException(L"IDXGIDevice::GetAdapter() failed.", hr);
	}
	if (FAILED(hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), &dxgiFactory))) {
		throw EngineException(L"IDXGIAdapter::GetParent() failed.", hr);
	}

	// Describe swap chain
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	ZeroMemory(&dxgiSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	dxgiSwapChainDesc.BufferCount							= 1;
	dxgiSwapChainDesc.BufferDesc.Width						= mViewportWidth;
	dxgiSwapChainDesc.BufferDesc.Height						= mViewportHeight;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator		= 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator	= 0;
	dxgiSwapChainDesc.BufferDesc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.OutputWindow							= mWndViewport;
	dxgiSwapChainDesc.SampleDesc.Count						= 1;
	dxgiSwapChainDesc.SampleDesc.Quality					= 0;
	dxgiSwapChainDesc.Windowed								= TRUE;

	if (FAILED(hr = dxgiFactory->CreateSwapChain(mDevice.Get(), &dxgiSwapChainDesc, &mSwapChain))) {
		throw EngineException(L"IDXGIFactory::CreateSwapChain() failed.", hr);
	}

	// Create render target view
	ComPtr<ID3D11Texture2D> backBuffer;
	if (FAILED(hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer))) {
		throw EngineException(L"IDXGISwapChain::GetBuffer() failed.", hr);
	}
	if (FAILED(hr = mDevice->CreateRenderTargetView(backBuffer.Get(), NULL, &mRenderTargetView))) {
		throw EngineException(L"ID3DDevice::CreateRenderTargetView() failed", hr);
	}

	// Create depth buffer (z-buffer)
	D3D11_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable		= TRUE;
	d3dDepthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencilDesc.DepthFunc		= D3D11_COMPARISON_LESS;

	d3dDepthStencilDesc.StencilEnable		= TRUE;
	d3dDepthStencilDesc.StencilReadMask		= D3D11_DEFAULT_STENCIL_READ_MASK;
	d3dDepthStencilDesc.StencilWriteMask	= D3D11_DEFAULT_STENCIL_WRITE_MASK;

	d3dDepthStencilDesc.FrontFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	d3dDepthStencilDesc.FrontFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	d3dDepthStencilDesc.BackFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	d3dDepthStencilDesc.BackFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;

	if (FAILED(hr = mDevice->CreateDepthStencilState(&d3dDepthStencilDesc, &mDepthStencilState))) {
		throw EngineException(L"ID3DDevice::CreateDepthDepthStencilState() failed.", hr);
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dDepthStencilViewDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Texture2D.MipSlice	= 0;

	ComPtr<ID3D11Texture2D> depthBuffer;
	D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
	ZeroMemory(&d3dTexture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dTexture2DDesc.Width				= mViewportWidth;
	d3dTexture2DDesc.Height				= mViewportHeight;
	d3dTexture2DDesc.MipLevels			= 1;
	d3dTexture2DDesc.ArraySize			= 1;
	d3dTexture2DDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dTexture2DDesc.SampleDesc.Count	= 1;
	d3dTexture2DDesc.SampleDesc.Quality = 0;
	d3dTexture2DDesc.Usage				= D3D11_USAGE_DEFAULT;
	d3dTexture2DDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	d3dTexture2DDesc.CPUAccessFlags		= 0;
	d3dTexture2DDesc.MiscFlags			= 0;


	if (FAILED(hr = mDevice->CreateTexture2D(&d3dTexture2DDesc, NULL, &depthBuffer))) {
		throw EngineException(L"ID3DDevice::CreateTexture2D() failed.", hr);
	}
	if (FAILED(hr = mDevice->CreateDepthStencilView(depthBuffer.Get(), &d3dDepthStencilViewDesc, &mDepthStencilView))) {
		throw EngineException(L"ID3DDevice::CreateDepthStencilView() failed.", hr);
	}

	D3D11_VIEWPORT viewport;
	viewport.Width = (float)mViewportWidth;
	viewport.Height = (float)mViewportHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	mContext->RSSetViewports(1, &viewport);
	mContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0);
	mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

	// Create WorldViewProjection constant buffer
	WorldViewProjection wvpCBuffer;
	ZeroMemory(&wvpCBuffer, sizeof(WorldViewProjection));

	D3D11_BUFFER_DESC wvpCBufferDesc;
	ZeroMemory(&wvpCBufferDesc, sizeof(D3D11_BUFFER_DESC));
	wvpCBufferDesc.ByteWidth			= sizeof(WorldViewProjection);
	wvpCBufferDesc.Usage				= D3D11_USAGE_DEFAULT;
	wvpCBufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	wvpCBufferDesc.CPUAccessFlags		= 0;
	wvpCBufferDesc.MiscFlags			= 0;
	wvpCBufferDesc.StructureByteStride	= 0;

	D3D11_SUBRESOURCE_DATA wvpCBufferData;
	ZeroMemory(&wvpCBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	wvpCBufferData.pSysMem			= &wvpCBuffer;
	wvpCBufferData.SysMemPitch		= 0;
	wvpCBufferData.SysMemSlicePitch = 0;

	if (FAILED(hr = mDevice->CreateBuffer(&wvpCBufferDesc, &wvpCBufferData, &mWorldViewProjectionCBuffer))) {
		throw EngineException(L"ID3D11Device::CreateBuffer() failed.", hr);
	}
	mContext->VSSetConstantBuffers(0, 1, mWorldViewProjectionCBuffer.GetAddressOf());
}

void Window::ClearBackBuffer() {
	mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0); // Clear the depth buffer by 1.0 and Stencil by 0
	mContext->ClearRenderTargetView(mRenderTargetView.Get(), gClearColor); // Clear render target by sky color
}

void Window::SwapBuffers() {
	mSwapChain->Present(0, 0);
}

ID3D11Device* Window::GetDevice() const {
	return mDevice.Get();
}

ID3D11DeviceContext* Window::GetContext() const {
	return mContext.Get();
}

ID3D11Buffer* Window::GetWVPCBuffer() const {
	return mWorldViewProjectionCBuffer.Get();
}