#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "GraphicComman.h"

#include <Windows.h>

#define DEFITEM MF_STRING
#define SUBMENU MF_STRING | MF_POPUP

typedef struct _menuitem {
	HMENU   hMenu;
	UINT    uFlag;
	LPCWSTR lpItemName;
} MENUITEM;

class Window {
public:
	Window(HINSTANCE instance, LPCWSTR className, LPCWSTR wndName, int width, int height, int vpWidth, int vpHeight, int nCmdShow);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	int GetWidth() const;
	int GetHeight() const;
	int GetViewportWidth() const;
	int GetViewportHeight() const;

	HINSTANCE GetInstance() const;
	HWND GetWnd() const;

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetContext() const;
	ID3D11Buffer* GetWVPCBuffer() const;

protected:
	void ClearBackBuffer(); // Clear back buffer of the swap chain
	void SwapBuffers(); // Swap buffers of the swap chain

	static void Close();
	static void ExtendWndProc(LRESULT(CALLBACK* func)(HWND, UINT, WPARAM, LPARAM));

	HWND CreateControl(DWORD deICC, LPCWSTR lpClassName, int x, int y, int width, int height);
	HMENU SubMenuBegin(LPCWSTR lpNameItem, UINT uType);
	HMENU MenuBegin();
	BOOL SubMenuEnd();
	BOOL MenuEnd();
	BOOL AddItem(UINT_PTR uIDNewItem, LPCWSTR lpNameItem, UINT uType);

private:
	void Initialize();
	void InitializeWindow();
	void InitializeDirectX();

	static LRESULT (CALLBACK* CustomWindowProc)(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK CustomDefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	LPCWSTR mClassName;
	LPCWSTR mWndName;

	HINSTANCE mInstance;
	HWND mWndViewport;
	HWND mWnd;

	int mWidth;
	int mHeight;
	int mViewportWidth;
	int mViewportHeight;
	int mCmdShow;

	D3D_FEATURE_LEVEL mLevel;
	ComPtr<ID3D11Device> mDevice;
	ComPtr<ID3D11DeviceContext> mContext;
	ComPtr<IDXGISwapChain> mSwapChain;
	ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	ComPtr<ID3D11DepthStencilView> mDepthStencilView;
	ComPtr<ID3D11DepthStencilState> mDepthStencilState;
	ComPtr<ID3D11Buffer> mWorldViewProjectionCBuffer;
};

#endif // !_WINDOW_H_
