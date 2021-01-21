#include "ObjReader.h"

#include <fstream>
#include <string>

#define VERTEX_BUFFER_SIZE 500000
#define INDEX_BUFFER_SIZE 1000000

ObjReader::ObjReader(LPCWSTR filename)
/*	: mV(VERTEX_BUFFER_SIZE), mVn(VERTEX_BUFFER_SIZE), mVt(VERTEX_BUFFER_SIZE),
	mVertexBuffer(VERTEX_BUFFER_SIZE), mIndexBuffer(INDEX_BUFFER_SIZE), mVertexVocabulary(VERTEX_BUFFER_SIZE)*/ {
	mVertexBuffer.reserve(VERTEX_BUFFER_SIZE);
	mIndexBuffer.reserve(INDEX_BUFFER_SIZE);
	mV.reserve(VERTEX_BUFFER_SIZE);
	mVn.reserve(VERTEX_BUFFER_SIZE);
	mVt.reserve(VERTEX_BUFFER_SIZE);
	mVertexVocabulary.reserve(VERTEX_BUFFER_SIZE);
	ReadFile(filename);
}

UINT ObjReader::GetVertexBufferSize() const {
	return mVertexBuffer.size();
}

UINT ObjReader::GetIndexBufferSize() const {
	return mIndexBuffer.size();
}

VERTEX* ObjReader::GetVertexBuffer() {
	return &mVertexBuffer[0];
}

UINT* ObjReader::GetIndexBuffer() {
	return &mIndexBuffer[0];
}

XMFLOAT4 ObjReader::ReadPosition(std::stringstream &istream) {
	float x, y, z, w = 1.0f;
	istream >> x >> y >> z >> w;
	return {x, y, z, w};
}

XMFLOAT3 ObjReader::ReadNormal(std::stringstream& istream) {
	float x, y, z;
	istream >> x >> y >> z;
	return {x, y, z};
}

XMFLOAT3 ObjReader::ReadTexel(std::stringstream& istream) {
	float u, v = 0.0f, w = 0.0f;
	istream >> u >> v >> w;
	return {u, v, w};
}

std::vector<std::string> ObjReader::ReadFace(std::stringstream& istream) {
	std::vector<std::string> face;
	std::string elemStorage;
	while (istream >> elemStorage) {
		face.push_back(elemStorage);
	}
	return face;
}

std::vector<UINT> ObjReader::CreateFaceElems(std::string &faceValue) {
	std::vector<UINT> faceElems;
	size_t startIdx = 0;
	for (int i = 0; i < 3; i++) {
		UINT faceElem = -1;
		if (startIdx < faceValue.size()) {
			char* eptr;
			faceElem = static_cast<UINT>(std::strtol(&faceValue[startIdx], &eptr, 10)) - 1;
			startIdx = eptr - faceValue.c_str() + 1;
		}
		faceElems.push_back(faceElem);
	}
	return faceElems;
}

VERTEX ObjReader::CreateVertex(std::vector<UINT> faceElems) {
	XMFLOAT4 position = mV[faceElems[0]];
	XMFLOAT3 texcoord = (faceElems[1] == -1) ? XMFLOAT3(0.0f, 0.0f, 0.0f) : mVt[faceElems[1]];
	XMFLOAT3 normal = (faceElems[2] == -1) ? XMFLOAT3(0.0f, 0.0f, 0.0f) : mVn[faceElems[2]];
	return { position, texcoord, normal };
}

void ObjReader::ExtendVertexBuffer(std::vector<std::string> &faceValue) {
	for (size_t i = 0; i < faceValue.size(); i++) {
		VERTEX vertex = CreateVertex(CreateFaceElems(faceValue[i]));
		if (mVertexVocabulary.find(faceValue[i]) == mVertexVocabulary.end()) {
			mVertexBuffer.push_back(vertex);
			mVertexVocabulary[faceValue[i]] = std::pair<UINT, VERTEX>(mVertexVocabulary.size(), vertex);
		}
	}
}

void ObjReader::ExtendIndexBuffer(std::vector<std::string> &faceValue) {
	UINT firstVertexIdx = mVertexVocabulary[faceValue[0]].first;
	UINT secondVertexIdx = mVertexVocabulary[faceValue[1]].first;
	UINT thirdVertexIdx = mVertexVocabulary[faceValue[2]].first;
	mIndexBuffer.push_back(firstVertexIdx);
	mIndexBuffer.push_back(secondVertexIdx);
	mIndexBuffer.push_back(thirdVertexIdx);
	for (size_t i = 3; i < faceValue.size(); i++) {
		UINT tempVertexIdx = mVertexVocabulary[faceValue[i]].first;
		mIndexBuffer.push_back(firstVertexIdx);
		mIndexBuffer.push_back(thirdVertexIdx);
		mIndexBuffer.push_back(tempVertexIdx);
		firstVertexIdx = secondVertexIdx;
		secondVertexIdx = thirdVertexIdx;
		thirdVertexIdx = tempVertexIdx;
	}
}

void ObjReader::ExtendBuffers(std::vector<std::string> faceValue) {
	ExtendVertexBuffer(faceValue);
	ExtendIndexBuffer(faceValue);
}

void ObjReader::CreateElem(UINT attribute, LPCSTR value) {
	std::stringstream ss(value);
	switch (attribute) {
		case 102: // f
			ExtendBuffers(ReadFace(ss));
			break;
		case 118: // v
			mV.push_back(ReadPosition(ss));
			break;
		case 228: // vn
			mVn.push_back(ReadNormal(ss));
			break;
		case 234: // vt
			mVt.push_back(ReadTexel(ss));
			break;
	}
}

void ObjReader::ReadFile(LPCWSTR filename) {
	std::ifstream in(filename);
	if (!in.is_open()) {
		return; // TODO Exception
	}

	std::string attribstr;
	while (in >> attribstr) {
		if (!attribstr.empty()) {
			std::string value;
			std::getline(in, value);
			CreateElem(attribstr[0] + attribstr[1], value.c_str());
		}
	}
	in.close();
}