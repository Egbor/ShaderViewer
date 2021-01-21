#ifndef _SHADER_H_
#define _SHADER_H_

#include "GraphicComman.h"

class Shader {
public:
	Shader(LPCWSTR pFileName, LPCSTR pEntryPoint, LPCSTR pTarget);

	ID3DBlob* GetBinary() const;
	LPCSTR GetError() const;

private:
	void Compile(LPCWSTR pFileName, LPCSTR pEntryPoint, LPCSTR pTarget);

	ComPtr<ID3DBlob> mShaderObj; // Compiled shader
	ComPtr<ID3DBlob> mShaderErr; // Shader error
};

#endif // !_SHADER_H_
