#ifndef _BUFFERS_H_
#define _BUFFERS_H_

#include "MathCommon.h"

struct WorldViewProjection {
	XMFLOAT4X4 mWorldViewProjection;
	XMFLOAT4X4 mWorld;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProjection;
};

#endif // !_BUFFERS_H_
