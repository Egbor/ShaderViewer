#ifndef _OBJ_READER_H_
#define _OBJ_READER_H_

#include "GraphicComman.h"

#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

class ObjReader {
public:
	ObjReader(LPCWSTR filename);

	UINT GetVertexBufferSize() const;
	UINT GetIndexBufferSize() const;

	VERTEX* GetVertexBuffer();
	UINT* GetIndexBuffer();

private:
	void ReadFile(LPCWSTR filename);
	void CreateElem(UINT attribute, LPCSTR value);
	void ExtendBuffers(std::vector<std::string> faceValues);
	void ExtendVertexBuffer(std::vector<std::string> &faceValues);
	void ExtendIndexBuffer(std::vector<std::string> &faceValues);
	VERTEX CreateVertex(std::vector<UINT> faceElems);

	static std::vector<UINT> CreateFaceElems(std::string& faceValue);

	static std::vector<std::string> ReadFace(std::stringstream& istream);
	static XMFLOAT4 ReadPosition(std::stringstream &istream);
	static XMFLOAT3 ReadNormal(std::stringstream &istream);
	static XMFLOAT3 ReadTexel(std::stringstream &istream);

	//UINT AddElemToVertexBuffer(VERTEX elem);

	std::vector<XMFLOAT4> mV;
	std::vector<XMFLOAT3> mVt;
	std::vector<XMFLOAT3> mVn;

	std::unordered_map<std::string, std::pair<UINT, VERTEX>> mVertexVocabulary;

	std::vector<UINT> mIndexBuffer;
	std::vector<VERTEX> mVertexBuffer;
};

#endif // !_OBJ_READER_H_
