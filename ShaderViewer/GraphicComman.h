#ifndef _GRAPHIC_COMMAN_H_
#define _GRAPHIC_COMMAN_H_

#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl/client.h> // For ComPtr
#include <WICTextureLoader.h>

#define RGBA_UNORM(r, g, b, a) { (float)r / (float)0xFF, (float)g / (float)0xFF, b / (float)0xFF, (float)a / (float)0xFF } 

using namespace Microsoft::WRL;

#include "MathCommon.h"

//static INT GetControlSum(void* data, size_t size) {
//	INT controlSum = 0;
//	for (size_t i = 0; i < size; i++) {
//		controlSum += ((i >> 4) | (controlSum << 4)) ^ (INT)((char*)data)[i];
//	}
//	return controlSum;
//}

typedef struct _vertex {
	XMFLOAT4 position;
	XMFLOAT3 texcoord;
	XMFLOAT3 normal;

	//bool operator<(const _vertex& v) const {
	//	return GetControlSum((void*)this, sizeof(_vertex)) < GetControlSum((void*)&v, sizeof(_vertex));/*(position.x < v.position.x) && (position.y < v.position.y) && (position.z < v.position.z) && (position.w < v.position.w) ||
	//		(texcoord.x < v.texcoord.x) && (texcoord.y < v.texcoord.y) && (texcoord.z < v.texcoord.z) ||
	//		(normal.x < v.normal.x) && (normal.y < v.normal.y) && (normal.z < v.normal.z);*/
	//}

	//bool operator==(const _vertex& v) const {
	//	return (position.x == v.position.x) && (position.y == v.position.y) && (position.z == v.position.z) && (position.w == v.position.w) &&
	//		(texcoord.x == v.texcoord.x) && (texcoord.y == v.texcoord.y) && (texcoord.z == v.texcoord.z) &&
	//		(normal.x == v.normal.x) && (normal.y == v.normal.y) && (normal.z == v.normal.z);
	//}
} VERTEX;

typedef struct _model {
	size_t vertexBufferSize;
	size_t indexBufferSize;
	VERTEX* vertexBuffer;
	UINT* indexBuffer;
} MODELBUFFER;

#endif // !_GRAPHIC_COMMAN_H_
