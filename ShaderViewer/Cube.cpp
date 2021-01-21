#include "Engine.h"
#include "EngineUtility.h"
#include "Shader.h"
#include "Vertex.h"
#include "Cube.h"

const Vertex vertexBuffer[] = {
	{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ XMFLOAT3( 0.5f,  0.5f, -0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ XMFLOAT3( 0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

	{ XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ XMFLOAT3( 0.5f,  0.5f,  0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3( 0.5f, -0.5f,  0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
};

const unsigned int indexBuffer[] = {
	0, 1, 3,
	3, 1, 2, // Front side

	7, 6, 4,
	4, 6, 5, // Back side

	4, 5, 0,
	0, 5, 1, // Left side

	3, 2, 7,
	7, 2, 6, // Right side

	1, 5, 2,
	2, 5, 6, // Top side

	4, 0, 7,
	7, 0, 3, // Bottom side
	//4, 5, 7,
	//7, 6, 5,
};

const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

Cube::Cube(Engine& engine, Camera& camera) : DrawableObject(engine, camera) {
	vertexShader = new Shader(L"test.hlsl", "vertex_shader", "vs_4_0");
	pixelShader = new Shader(L"test.hlsl", "pixel_shader", "ps_4_0");
}

Cube::~Cube() {
	DELETE_OBJECT(vertexShader);
	DELETE_OBJECT(pixelShader);
}

void Cube::Draw() {
	ID3D11DeviceContext* context = mEngine->GetContext();
	SetVertexBuffer(vertexBuffer, 8, sizeof(Vertex));
	SetIndexBuffer(indexBuffer, 36);
	SetShaders(inputElementDesc, 2, vertexShader->GetBinary(), pixelShader->GetBinary());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->DrawIndexed(36, 0, 0);
}