#include "Engine.h"
#include "EngineUtility.h"
#include "EngineException.h"

#include "ObjReader.h"
#include "Model.h"

Model::Model(Engine& engine, Camera& camera) : DrawableObject(engine, camera) {
	Super::SetSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
}

void Model::SetShaders(Shader& vertexShader, Shader& pixelShader) {
	Super::SetShaders(inputElementDesc, 3, vertexShader.GetBinary(), pixelShader.GetBinary());
}

void Model::SetModelFromFile(LPCWSTR filename) {
	ObjReader reader(filename);
	Super::SetVertexBuffer(reader.GetVertexBuffer(), reader.GetVertexBufferSize(), sizeof(VERTEX));
	Super::SetIndexBuffer(reader.GetIndexBuffer(), reader.GetIndexBufferSize());
}

void Model::SetTextureFromFile(LPCWSTR filename) {
	ID3D11Device* device = mEngine->GetDevice();
	ID3D11ShaderResourceView* mTextureView;
	HRESULT hr;
	if (FAILED(hr = CreateWICTextureFromFile(device, filename, NULL, &mTextureView))) {
		throw EngineException(L"DirectX::CreateWICTextureFromFile() failed.", hr);
	}
	Super::SetShaderResource(mTextureView);
}

void Model::Draw() {
	Super::Draw();

	ID3D11DeviceContext* d3dContext = mEngine->GetContext();
	d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3dContext->DrawIndexed(mIBIndexCount, 0, 0);
}