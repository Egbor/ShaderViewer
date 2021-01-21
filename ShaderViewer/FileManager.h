#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_

#include <Windows.h>

class FileManager {
public:
	FileManager() = default;

	LPCWSTR Open(HWND hWnd, LPWSTR lpstrFile, LPWSTR lpstrTitle, DWORD nMaxFile, DWORD nMaxTitle, LPCWSTR lpstrFilter);
};

#endif // !_FILE_MANAGER_H_
