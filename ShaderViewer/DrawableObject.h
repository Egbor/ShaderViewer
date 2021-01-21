#ifndef _DRAWABLE_OBJECT_H_
#define _DRAWABLE_OBJECT_H_

#include "Camera.h"
#include "Buffers.h"
#include "Shader.h"

#include "Object.h"

class DrawableObject : public Object {
public:
	using Super = Object;

	DrawableObject(Engine &engine, Camera &camera);

	virtual void Draw();
	virtual void Update(Time tick);

protected:
	void SetShaders(const D3D11_INPUT_ELEMENT_DESC* pInputElementDesc, UINT numElem, ID3DBlob* pVertexCode, ID3DBlob* pPixelCode);
	void SetShaderResource(ID3D11ShaderResourceView* pShaderResourceView);
	void SetVertexBuffer(const void* buffer, size_t numElem, size_t typeSize);
	void SetIndexBuffer(const void* buffer, size_t numElem);
	void SetSampler(const D3D11_FILTER filter, const D3D11_TEXTURE_ADDRESS_MODE address);

	static const D3D11_INPUT_ELEMENT_DESC inputElementDesc[];

	UINT mVBStrides;
	UINT mVBOffsets;
	UINT mIBIndexCount;

	Camera* mCamera;

private:
	void SetInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDesc, UINT numElem, const void* pBytecode, size_t bytecodeLenght);
	void SetVertexShader(const void* pBytecode, size_t bytecodeLenght);
	void SetPixelShader(const void* pBytecode, size_t bytecodeLenght);

	ComPtr<ID3D11Buffer> mVertexBuffer;
	ComPtr<ID3D11Buffer> mIndexBuffer;
	ComPtr<ID3D11InputLayout> mInputLayout;
	ComPtr<ID3D11VertexShader> mVertexShader;
	ComPtr<ID3D11PixelShader> mPixelShader;
	ComPtr<ID3D11ShaderResourceView> mTextureView;
	ComPtr<ID3D11SamplerState> mSamplerState;
};

#endif // !_DRAWABLE_OBJECT_H_
