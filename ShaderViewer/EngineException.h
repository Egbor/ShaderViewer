#ifndef _ENGINE_EXCEPTION_H_
#define _ENGINE_EXCEPTION_H_

#include <Windows.h>
#include <exception>
#include <string>

class EngineException : public std::exception {
public:
	EngineException(LPCWSTR message, HRESULT hr = 0);

	virtual std::wstring info();
private:
	LPCWSTR msg;
	HRESULT code;
};

#endif // !_ENGINE_EXCEPTION_H_
