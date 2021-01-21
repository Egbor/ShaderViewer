#ifndef _SHADER_VIEVER_H_
#define _SHADER_VIEVER_H_

#include "Engine.h"
#include "FileManager.h"
#include "Model.h"
#include "CameraController.h"

#include <list>
#include <vector>

#define ID_IMPORT 0x101
#define ID_EXIT 0x102

#define BN_IMPORT_TEXTURE 0x103
#define BN_IMPORT_SHADER 0x104

typedef struct _item {
	Model model;
	WCHAR textureFilename[256];
	WCHAR shaderFilename[256];
} ITEM;

class ShaderViewer : public Engine {
public:
	ShaderViewer(HINSTANCE instance, LPCWSTR className, LPCWSTR wndName, int width, int height, int vpWidth, int vpHeight, int nCmdShow);
	~ShaderViewer();

private:
	void Initialize();
	void InitializeViewport();
	void InitializeMainMenu();
	void InitializeControls();
	void InitializeTabs();
	void InitializeTabsControls();
	void InitializeHandlers();

	void CreateTabItem(WCHAR* title, int index);
	void CreateTabControl(LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD style, DWORD id, int x, int y, int width, int height, int index);

	void SwitchTab();
	void UpdateTab();
	BOOL ShowTabControl(int tabId, int ctrlId, int nCmdShow);

	void AddObjectToListBox(LPCWSTR title);
	void RemoveObjectFromListBox(int index);

	FLOAT ReadTransformEdit(HWND edit);
	void WriteTransformEdit(HWND edit, FLOAT value);
	void ChangeSelectedObjectPosition();
	void ChangeSelectedObjectRotation();
	void ChangeSelectedObjectScale();
	void ChangeSelectedObjectTransform();

	void ImportTxeture(HWND hWnd);
	void ImportShader(HWND hWnd);

	void LoadTexture(LPCWSTR filename);
	void LoadShader(LPCWSTR filename);
	void LoadModel(LPCWSTR filename, LPCWSTR modelname);
	void DeleteModel();

	//void AppandModelView(LPWSTR title);
	std::list<ITEM>::iterator GetItem(int id);

	HWND mWndModelView;

	Camera mCamera;
	Shader mDefaultVertexShader;
	Shader mDefaultPixelShader;

	CameraController mCamCtrl;

	std::list<ITEM> mItems;
	std::vector<std::vector<HWND>> mTabWndCtrl;

private:
	static LRESULT CALLBACK HandleWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleTabCtrl(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleListBox(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void ImportModel(HWND hWnd);

	static WNDPROC mMainWndHandler;
	static WNDPROC mTabCtrlHandler;

	static FileManager mFileManager;
	static ShaderViewer* mEngine;
};

#endif // !_SHADER_VIEVER_H_
