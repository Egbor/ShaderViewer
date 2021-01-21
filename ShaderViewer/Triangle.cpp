#include "Engine.h"
#include "EngineUtility.h"
#include "Vertex.h"
#include "Triangle.h"

const Vertex vertexBuffer[] = {
	{ XMFLOAT3( 0.0f,  0.5f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3( 0.5f, -0.5f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
};

const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

Triangle::Triangle(Engine &engine, Camera &camera) : DrawableObject(engine, camera) {
	vertexShader = new Shader(L"test.hlsl", "vertex_shader", "vs_4_0");
	pixelShader = new Shader(L"test.hlsl", "pixel_shader", "ps_4_0");
}

Triangle::~Triangle() {
	DELETE_OBJECT(vertexShader);
	DELETE_OBJECT(pixelShader);
}

void Triangle::Draw() {
	ID3D11DeviceContext* context = mEngine->GetContext();
	SetVertexBuffer(vertexBuffer, 3, sizeof(Vertex));
	SetShaders(inputElementDesc, 2, vertexShader->GetBinary(), pixelShader->GetBinary());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->Draw(3, 0);
}