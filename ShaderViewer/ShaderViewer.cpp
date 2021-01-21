#include "EngineException.h"
#include "ShaderViewer.h"

#include "CameraController.h"

#include <CommCtrl.h>
#include <windowsx.h>
#include <regex>
#include <sstream>

FileManager ShaderViewer::mFileManager;
ShaderViewer* ShaderViewer::mEngine;

WNDPROC ShaderViewer::mMainWndHandler;
WNDPROC ShaderViewer::mTabCtrlHandler;

ShaderViewer::ShaderViewer(HINSTANCE instance, LPCWSTR className, LPCWSTR wndName, int width, int height, int vpWidth, int vpHeight, int nCmdShow)
	: Engine(instance, className, wndName, width, height, vpWidth, vpHeight, nCmdShow), mWndModelView(NULL), mCamera(*this), mTabWndCtrl(2), mCamCtrl(mCamera),
	  mDefaultVertexShader(L"..\\Shaders\\phong.hlsl", "vertex_shader", "vs_4_0"), mDefaultPixelShader(L"..\\Shaders\\phong.hlsl", "pixel_shader", "ps_4_0") {
	Initialize();
}

ShaderViewer::~ShaderViewer() {
	//if (camCtrl != nullptr) {
	//	delete camCtrl;
	//}
}

void ShaderViewer::Initialize() {
	InitializeViewport();
	InitializeMainMenu();
	InitializeControls();
	InitializeTabs();
	InitializeTabsControls();
	InitializeHandlers();
	//ExtendWndProc(HandleWndProc);
}

void ShaderViewer::InitializeViewport() {
	mEngine = this;
	mEngine->SetMainCamera(mCamera);
	mEngine->AddScript(mCamCtrl);

	mCamera.transform.Translate(0.0f, 0.0f, -10.0f); // Delete?
}

void ShaderViewer::InitializeMainMenu() {
	MenuBegin();
		SubMenuBegin(L"File", SUBMENU);
			AddItem(ID_IMPORT, L"Import", DEFITEM);
			AddItem(ID_EXIT, L"Exit", DEFITEM);
		SubMenuEnd();
	MenuEnd();
	//SetMainMenuHandler(HandleMainMenu);
}

void ShaderViewer::InitializeControls() {
	int modelX = GetViewportWidth();
	int modelY = GetViewportHeight();
	int modelControlWidth = GetWidth() - modelX;
	int modelControlHeight = modelY;

	mWndModelView = CreateControl(ICC_TAB_CLASSES, WC_TABCONTROL, modelX, 0, modelControlWidth, modelControlHeight);
}

void ShaderViewer::InitializeTabs() {
	WCHAR viewTitle[] = L"Models";
	WCHAR paramTitle[] = L"Property";
	CreateTabItem(viewTitle, 0);
	CreateTabItem(paramTitle, 1);
}

void ShaderViewer::InitializeTabsControls() {
	CreateTabControl(WC_LISTBOX, L"", WS_BORDER, NULL, 4, 34, 241, 495, 0);
	CreateTabControl(WC_STATIC, L"Position", NULL, 0, 4, 34, 80, 25, 1);
	CreateTabControl(WC_STATIC, L"Rotation", NULL, 0, 4, 64, 80, 25, 1);
	CreateTabControl(WC_STATIC, L"Scale", 0, NULL, 4, 94, 80, 25, 1);
	CreateTabControl(WC_STATIC, L"Texture", NULL, 0, 4, 131, 80, 25, 1);
	CreateTabControl(WC_STATIC, L"Shader", NULL, 0, 4, 198, 80, 25, 1);
	CreateTabControl(WC_EDIT, L"0.0", WS_BORDER | ES_AUTOHSCROLL, NULL, 84, 34, 40, 25, 1);
	CreateTabControl(WC_EDIT, L"0.0", WS_BORDER | ES_AUTOHSCROLL, NULL, 84, 64, 40, 25, 1);
	CreateTabControl(WC_EDIT, L"1.0", WS_BORDER | ES_AUTOHSCROLL, NULL, 84, 94, 40, 25, 1);
	CreateTabControl(WC_EDIT, L"0.0", WS_BORDER | ES_AUTOHSCROLL, NULL, 125, 34, 40, 25, 1);
	CreateTabControl(WC_EDIT, L"0.0", WS_BORDER | ES_AUTOHSCROLL, NULL, 125, 64, 40, 25, 1);
	CreateTabControl(WC_EDIT, L"1.0", WS_BORDER | ES_AUTOHSCROLL, NULL, 125, 94, 40, 25, 1);
	CreateTabControl(WC_EDIT, L"0.0", WS_BORDER | ES_AUTOHSCROLL, NULL, 166, 34, 40, 25, 1);
	CreateTabControl(WC_EDIT, L"0.0", WS_BORDER | ES_AUTOHSCROLL, NULL, 166, 64, 40, 25, 1);
	CreateTabControl(WC_EDIT, L"1.0", WS_BORDER | ES_AUTOHSCROLL, NULL, 166, 94, 40, 25, 1);
	CreateTabControl(WC_EDIT, L"", WS_BORDER | ES_READONLY | ES_AUTOHSCROLL, NULL, 84, 130, 122, 25, 1);
	CreateTabControl(WC_EDIT, L"", WS_BORDER | ES_READONLY | ES_AUTOHSCROLL, NULL, 84, 198, 122, 25, 1);
	CreateTabControl(WC_BUTTON, L"Load", WS_BORDER, BN_IMPORT_TEXTURE, 126, 160, 80, 25, 1);
	CreateTabControl(WC_BUTTON, L"Load", WS_BORDER, BN_IMPORT_SHADER, 126, 228, 80, 25, 1);
	SwitchTab();
}

void ShaderViewer::InitializeHandlers() {
	mMainWndHandler = (WNDPROC)SetWindowLong(mWndModelView, GWL_WNDPROC, (DWORD)HandleTabCtrl);
	mTabCtrlHandler = (WNDPROC)SetWindowLong(mTabWndCtrl[0][0], GWL_WNDPROC, (DWORD)HandleListBox);
	ExtendWndProc(HandleWndProc);
}

void ShaderViewer::CreateTabItem(WCHAR* title, int index) {
	TCITEM tcItem;
	ZeroMemory(&tcItem, sizeof(tcItem));
	tcItem.mask = TCIF_TEXT | TCIF_IMAGE;
	tcItem.pszText = title;
	tcItem.iImage = -1;
	TabCtrl_InsertItem(mWndModelView, index, &tcItem);
}

void ShaderViewer::CreateTabControl(LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD style, DWORD id, int x, int y, int width, int height, int index) {
	HWND hWnd;
	if ((hWnd = CreateWindow(lpClassName, lpWindowName, WS_CHILD | WS_VISIBLE | style, x, y, width, height, mWndModelView, (HMENU)id, GetInstance(), NULL)) == NULL) {
		throw EngineException(L"CreateTabControl() failed.");
	}
	mTabWndCtrl[index].push_back(hWnd);
}

BOOL ShaderViewer::ShowTabControl(int tabId, int ctrlId, int nCmdShow) {
	if ((tabId == 1) && (ListBox_GetCurSel(mTabWndCtrl[0][0]) < 0)) {
		return ShowWindow(mTabWndCtrl[tabId][ctrlId], SW_HIDE);
	}
	return ShowWindow(mTabWndCtrl[tabId][ctrlId], nCmdShow);
}

void ShaderViewer::SwitchTab() {
	int tabId = TabCtrl_GetCurSel(mWndModelView);
	for (int i = 0; i < mTabWndCtrl.size(); i++) {
		for (int j = 0; j < mTabWndCtrl[i].size(); j++) {
			ShowTabControl(i, j, (i == tabId) ? SW_SHOW : SW_HIDE);
		}
	}
}

void ShaderViewer::UpdateTab() {
	int index = ListBox_GetCurSel(mTabWndCtrl[0][0]);
	if (index > -1) {
		auto it = GetItem(index);

		Vector3 position = it->model.transform.GetPosition();
		Vector3 rotation = it->model.transform.GetRotation().ToEulerAngles();
		Vector3 scale = it->model.transform.GetScale();

		WriteTransformEdit(mTabWndCtrl[1][5], position.GetX());
		WriteTransformEdit(mTabWndCtrl[1][8], position.GetY());
		WriteTransformEdit(mTabWndCtrl[1][11], position.GetZ());

		WriteTransformEdit(mTabWndCtrl[1][6], RADIANS_TO_DEGREES(rotation.GetX()));
		WriteTransformEdit(mTabWndCtrl[1][9], RADIANS_TO_DEGREES(rotation.GetY()));
		WriteTransformEdit(mTabWndCtrl[1][12], RADIANS_TO_DEGREES(rotation.GetZ()));

		WriteTransformEdit(mTabWndCtrl[1][7], scale.GetX());
		WriteTransformEdit(mTabWndCtrl[1][10], scale.GetY());
		WriteTransformEdit(mTabWndCtrl[1][13], scale.GetZ());

		Edit_SetText(mTabWndCtrl[1][14], it->textureFilename);
		Edit_SetText(mTabWndCtrl[1][15], it->shaderFilename);
	}
}

FLOAT ShaderViewer::ReadTransformEdit(HWND edit) {
	WCHAR buffer[256] = { 0 };
	Edit_GetText(edit, buffer, 256);

	std::wregex regexFloat(L"-?\\d+(\\.\\d+)?");
	if (std::regex_match(std::wstring(buffer), regexFloat)) {
		return std::stof(buffer);
	}
	return 0.0f;
}

void ShaderViewer::WriteTransformEdit(HWND edit, FLOAT value) {
	std::wstringstream ss;
	ss << value;
	Edit_SetText(edit, ss.str().c_str());
}

void ShaderViewer::ChangeSelectedObjectPosition() {
	auto it = GetItem(ListBox_GetCurSel(mTabWndCtrl[0][0]));

	float positionX = ReadTransformEdit(mTabWndCtrl[1][5]);
	float positionY = ReadTransformEdit(mTabWndCtrl[1][8]);
	float positionZ = ReadTransformEdit(mTabWndCtrl[1][11]);

	it->model.transform.SetPosition(positionX, positionY, positionZ);
}

void ShaderViewer::ChangeSelectedObjectRotation() {
	auto it = GetItem(ListBox_GetCurSel(mTabWndCtrl[0][0]));

	float rotationX = DEGREES_TO_RADIANS(ReadTransformEdit(mTabWndCtrl[1][6]));
	float rotationY = DEGREES_TO_RADIANS(ReadTransformEdit(mTabWndCtrl[1][9]));
	float rotationZ = DEGREES_TO_RADIANS(ReadTransformEdit(mTabWndCtrl[1][12]));

	it->model.transform.SetRotation(rotationX, rotationY, rotationZ);
}

void ShaderViewer::ChangeSelectedObjectScale() {
	auto it = GetItem(ListBox_GetCurSel(mTabWndCtrl[0][0]));

	float scaleX = ReadTransformEdit(mTabWndCtrl[1][7]);
	float scaleY = ReadTransformEdit(mTabWndCtrl[1][10]);
	float scaleZ = ReadTransformEdit(mTabWndCtrl[1][13]);

	it->model.transform.SetScale(scaleX, scaleY, scaleZ);
}

void ShaderViewer::ChangeSelectedObjectTransform() {
	ChangeSelectedObjectPosition();
	ChangeSelectedObjectRotation();
	ChangeSelectedObjectScale();
}

void ShaderViewer::AddObjectToListBox(LPCWSTR title) {
	ListBox_AddString(mTabWndCtrl[0][0], title);
}

void ShaderViewer::RemoveObjectFromListBox(int index) {
	ListBox_DeleteString(mTabWndCtrl[0][0], index);
}

void ShaderViewer::DeleteModel() {
	int index = ListBox_GetCurSel(mTabWndCtrl[0][0]);
	if (index > -1) {
		auto it = GetItem(index);
		RemoveObjectFromListBox(index);
		DeleteObject(it->model);
		mItems.erase(it);
	}
}

void ShaderViewer::LoadModel(LPCWSTR filename, LPCWSTR modelname) {
	ITEM item = { Model(*this, mCamera), L"", L"" };
	item.model.SetModelFromFile(filename);
	item.model.SetShaders(mDefaultVertexShader, mDefaultPixelShader);

	mItems.push_back(item);
	AddObject(mItems.back().model);
	AddObjectToListBox(modelname);
}

void ShaderViewer::LoadTexture(LPCWSTR filename) {
	auto it = GetItem(ListBox_GetCurSel(mTabWndCtrl[0][0]));
	it->model.SetTextureFromFile(filename);
	std::memcpy(it->textureFilename, filename, 256);
}

void ShaderViewer::LoadShader(LPCWSTR filename) {
	auto it = GetItem(ListBox_GetCurSel(mTabWndCtrl[0][0]));
	Shader vertexShader(filename, "vertex_shader", "vs_4_0");
	Shader pixelShader(filename, "pixel_shader", "ps_4_0");
	it->model.SetShaders(vertexShader, pixelShader);
	std::memcpy(it->shaderFilename, filename, 256);
}

void ShaderViewer::ImportModel(HWND hWnd) {
	TCHAR filename[256] = { 0 };
	TCHAR modelname[256] = { 0 };
	if (mFileManager.Open(hWnd, filename, modelname, 256, 256, L"Wavefront(.obj)\0*.obj\0") != NULL) {
		mEngine->LoadModel(filename, modelname);
	}
}

void ShaderViewer::ImportTxeture(HWND hWnd) {
	TCHAR filename[256] = { 0 };
	if (mFileManager.Open(hWnd, filename, NULL, 256, 0, L"PNG(.png)\0*.png\0JPG(.jpg)\0*.jpg\0BMP(.bmp)\0*.bmp\0")) {
		LoadTexture(filename);
	}
}

void ShaderViewer::ImportShader(HWND hWnd) {
	TCHAR filename[256] = { 0 };
	if (mFileManager.Open(hWnd, filename, NULL, 256, 0, L"HLSL(.hlsl)\0*.hlsl\0")) {
		LoadShader(filename);
	}
}

std::list<ITEM>::iterator ShaderViewer::GetItem(int id) {
	std::list<ITEM>::iterator it = mItems.begin();
	std::advance(it, id);
	return it;
}

LRESULT CALLBACK ShaderViewer::HandleWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case ID_IMPORT:
					ImportModel(hWnd);
					break;
				case ID_EXIT:
					Close();
					break;
			}
			return 0;
		case WM_NOTIFY:
			switch (((LPNMHDR)lParam)->code) {
				case TCN_SELCHANGE:
					mEngine->SwitchTab();
					mEngine->UpdateTab();
					break;
			}
			return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK ShaderViewer::HandleTabCtrl(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_COMMAND:
			switch (HIWORD(wParam)) {
				case EN_CHANGE:
					mEngine->ChangeSelectedObjectTransform();
					break;
				case BN_CLICKED:
					OutputDebugString(L"Clicked");
					switch (LOWORD(wParam)) {
						case BN_IMPORT_TEXTURE:
							mEngine->ImportTxeture(hWnd);
							break;
						case BN_IMPORT_SHADER:
							mEngine->ImportShader(hWnd);
							break;
					}
					mEngine->UpdateTab();
					break;
			}
			return 0;
		default:
			return CallWindowProc(mMainWndHandler, hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK ShaderViewer::HandleListBox(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_KEYDOWN:
			switch (wParam) {
				case VK_DELETE:
					mEngine->DeleteModel();
					break;
			}
			return 0;
		default:
			return CallWindowProc(mTabCtrlHandler, hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}