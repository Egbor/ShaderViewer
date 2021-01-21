#include "EngineException.h"

#include <sstream>

EngineException::EngineException(LPCWSTR message, HRESULT hr) : msg(message), code(hr) {
}

std::wstring EngineException::info() {
	std::wostringstream wss;				 // Create wstring output stream
	wss << std::hex << code << L": " << msg; // Put all information in stream
	return wss.str();						 // Return wstring
}