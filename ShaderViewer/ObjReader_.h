#ifndef _OBJ_READER_H_
#define _OBJ_READER_H_

#include "GraphicComman.h"

#include <vector>
//#include <unordered_map>

class ObjReader {
public:
	ObjReader(const char* filename);

	MODELBUFFER GetModel();
	void SaveLogFile(const char* filename);

private:
	void CreateModelVertexBuffer(MODELBUFFER* model);
	void CreateModelIndexBuffer(MODELBUFFER* model);

	void ReadFile(const char* filename);
	void ReadAttributeLine(int attributeId, const char* attributeLine);

	static XMFLOAT4 ReadVertexAttributeLine(const char* attributeLine);
	static XMFLOAT3 ReadNormalAttributeLine(const char* attributeLine);
	static XMFLOAT3 ReadTextureAttributeLine(const char* attributeLine);
	static std::vector<INT> ReadFaceAttributeLine(const char* attributeLine);

	std::vector<XMFLOAT4> mV;
	std::vector<XMFLOAT3> mVt;
	std::vector<XMFLOAT3> mVn;
	std::vector<std::vector<INT>> mF;

	//std::vector<UINT> mIndexBuffer;
	//std::unordered_map<VERTEX, UINT> mVertexBuffer;
	UINT mVertexAmount;
};

#endif // !_OBJ_READER_H_
