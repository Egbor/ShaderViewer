#include "EngineException.h"
#include "FileManager.h"

LPCWSTR FileManager::Open(HWND hWnd, LPWSTR lpstrFile, LPWSTR lpstrTitle, DWORD nMaxFile, DWORD nMaxTitle, LPCWSTR lpstrFilter) {
	OPENFILENAME openFileName;
	ZeroMemory(&openFileName, sizeof(OPENFILENAME));
	openFileName.lStructSize	 = sizeof(OPENFILENAME);
	openFileName.hwndOwner		 = hWnd;
	openFileName.lpstrFile		 = lpstrFile;
	openFileName.nMaxFile		 = nMaxFile;
	openFileName.lpstrFilter	 = lpstrFilter;
	openFileName.nFilterIndex	 = 1;
	openFileName.lpstrFileTitle	 = lpstrTitle;
	openFileName.nMaxFileTitle	 = nMaxTitle;
	openFileName.lpstrInitialDir = NULL;
	openFileName.Flags			 = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&openFileName)) {
		return openFileName.lpstrFile;
	}
	return NULL;
}