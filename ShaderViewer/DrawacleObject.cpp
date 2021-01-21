#include "Engine.h"
#include "EngineException.h"

const D3D11_INPUT_ELEMENT_DESC DrawableObject::inputElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

DrawableObject::DrawableObject(Engine &engine, Camera &camera) 
	: Object(engine), mCamera(&camera), mVertexBuffer(), mIndexBuffer(), mInputLayout(),
	  mVertexShader(), mPixelShader(), mVBOffsets(0), mVBStrides(0), mIBIndexCount(0) {

}

void DrawableObject::Draw() {
	ID3D11DeviceContext* context = mEngine->GetContext();
	context->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &mVBStrides, &mVBOffsets);
	context->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetInputLayout(mInputLayout.Get());
	context->VSSetShader(mVertexShader.Get(), NULL, 0);
	context->PSSetShader(mPixelShader.Get(), NULL, 0);

	context->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());
	context->PSSetShaderResources(0, 1, mTextureView.GetAddressOf());
}

void DrawableObject::Update(Time tick) {

}

void DrawableObject::SetSampler(const D3D11_FILTER filter, const D3D11_TEXTURE_ADDRESS_MODE address) {
	ID3D11Device* device = mEngine->GetDevice();

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = filter;
	sampDesc.AddressU = address;
	sampDesc.AddressV = address;
	sampDesc.AddressW = address;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr;
	if (FAILED(hr = device->CreateSamplerState(&sampDesc, &mSamplerState))) {
		throw EngineException(L"ID3D11Device::CreateSamplerState() failed", hr);
	}
}

void DrawableObject::SetShaderResource(ID3D11ShaderResourceView* pShaderResourceView) {
	mTextureView.Attach(pShaderResourceView);
}

void DrawableObject::SetVertexBuffer(const void* buffer, size_t numElem, size_t typeSize) {
	ID3D11Device* device = mEngine->GetDevice();

	D3D11_BUFFER_DESC d3dVertexBufferDesc;
	ZeroMemory(&d3dVertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dVertexBufferDesc.ByteWidth		= numElem * typeSize;
	d3dVertexBufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	d3dVertexBufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	d3dVertexBufferDesc.CPUAccessFlags	= 0;
	d3dVertexBufferDesc.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA d3dSubresourceData;
	ZeroMemory(&d3dSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dSubresourceData.pSysMem			= buffer;
	d3dSubresourceData.SysMemPitch		= 0;
	d3dSubresourceData.SysMemSlicePitch = 0;

	HRESULT hr = 0;
	if (FAILED(hr = device->CreateBuffer(&d3dVertexBufferDesc, &d3dSubresourceData, &mVertexBuffer))) {
		throw EngineException(L"ID3D11Device::CreateBuffer() failed.", hr);
	}
	mVBStrides = typeSize;
	mVBOffsets = 0;
}

void DrawableObject::SetIndexBuffer(const void* buffer, size_t numElem) {
	ID3D11Device* device = mEngine->GetDevice();

	D3D11_BUFFER_DESC d3dIndexBufferDesc;
	ZeroMemory(&d3dIndexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dIndexBufferDesc.ByteWidth		= numElem * sizeof(UINT);
	d3dIndexBufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	d3dIndexBufferDesc.BindFlags		= D3D11_BIND_INDEX_BUFFER;
	d3dIndexBufferDesc.CPUAccessFlags	= 0;
	d3dIndexBufferDesc.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA d3dSubresourceData;
	ZeroMemory(&d3dSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dSubresourceData.pSysMem			= buffer;
	d3dSubresourceData.SysMemPitch		= 0;
	d3dSubresourceData.SysMemSlicePitch = 0;

	HRESULT hr = 0;
	if (FAILED(hr = device->CreateBuffer(&d3dIndexBufferDesc, &d3dSubresourceData, &mIndexBuffer))) {
		throw EngineException(L"ID3D11Device::CreateBuffer() failed.", hr);
	}
	mIBIndexCount = numElem;
}

void DrawableObject::SetShaders(const D3D11_INPUT_ELEMENT_DESC* pInputElementDesc, UINT numElem, ID3DBlob* pVertexCode, ID3DBlob* pPixelCode) {
	if (pVertexCode != NULL) {
		SetInputLayout(pInputElementDesc, numElem, pVertexCode->GetBufferPointer(), pVertexCode->GetBufferSize());
		SetVertexShader(pVertexCode->GetBufferPointer(), pVertexCode->GetBufferSize());
	}
	if (pPixelCode != NULL) {
		SetPixelShader(pPixelCode->GetBufferPointer(), pPixelCode->GetBufferSize());
	}
}

void DrawableObject::SetInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDesc, UINT numElem, const void* pBytecode, size_t bytecodeLenght) {
	ID3D11Device* device = mEngine->GetDevice();

	HRESULT hr = 0;
	if (FAILED(hr = device->CreateInputLayout(pInputElementDesc, numElem, pBytecode, bytecodeLenght, &mInputLayout))) {
		throw EngineException(L"ID3D11Device::CreateInputLayout() failed.", hr);
	}
}

void DrawableObject::SetVertexShader(const void* pBytecode, size_t bytecodeLenght) {
	ID3D11Device* device = mEngine->GetDevice();

	HRESULT hr = 0;
	if (FAILED(hr = device->CreateVertexShader(pBytecode, bytecodeLenght, NULL, &mVertexShader))) {
		throw EngineException(L"ID3D11Device::CreateVertexShader() failed.", hr);
	}
}

void DrawableObject::SetPixelShader(const void* pBytecode, size_t bytecodeLenght) {
	ID3D11Device* device = mEngine->GetDevice();

	HRESULT hr = 0;
	if (FAILED(hr = device->CreatePixelShader(pBytecode, bytecodeLenght, NULL, &mPixelShader))) {
		throw EngineException(L"ID3D11Device::CreatePixelShader() failed.", hr);
	}
}