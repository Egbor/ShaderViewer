#ifndef _MODEL_FILE_COMMON_
#define _MODEL_FILE_COMMON_

struct Vertex {
	float x;
	float y;
	float z;
	float w;
};

struct VertexNormal {
	float x;
	float y;
	float z;
};

struct Face {
	unsigned int v;
	unsigned int vt;
	unsigned int vn;
};

#endif // !_MODEL_FILE_COMMON_
