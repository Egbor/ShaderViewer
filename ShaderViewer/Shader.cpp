#include "Shader.h"
#include "EngineException.h"

Shader::Shader(LPCWSTR pFileName, LPCSTR pEntryPoint, LPCSTR pTarget) : mShaderObj(NULL), mShaderErr(NULL) {
	Compile(pFileName, pEntryPoint, pTarget);
}

void Shader::Compile(LPCWSTR pFileName, LPCSTR pEntryPoint, LPCSTR pTarget) {
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	flags = D3DCOMPILE_DEBUG;
#endif

	HRESULT hr;
	if (FAILED(hr = D3DCompileFromFile(pFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryPoint, pTarget, flags, 0, &mShaderObj, &mShaderErr))) {
		OutputDebugStringA(GetError());
		throw EngineException(L"Compilation error.\nD3DCompileFromFile() filed.", hr);
	}
}

ID3DBlob* Shader::GetBinary() const {
	return mShaderObj.Get();
}

LPCSTR Shader::GetError() const {
	return (const char*)mShaderErr->GetBufferPointer();
}