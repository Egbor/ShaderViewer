#include "EngineException.h"
#include "ObjReader.h"

#include <fstream>
#include <sstream>
#include <string>

ObjReader::ObjReader(const char* filename) : mVertexAmount(0) {
	ReadFile(filename);
}

XMFLOAT4 ObjReader::ReadVertexAttributeLine(const char* attributeLine) {
	float x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f;
	std::stringstream ss(attributeLine);
	ss >> x >> y >> z >> w;
	return XMFLOAT4(x, y, z, w);
}

XMFLOAT3 ObjReader::ReadNormalAttributeLine(const char* attributeLine) {
	float x = 0.0f, y = 0.0f, z = 0.0f;
	std::stringstream ss(attributeLine);
	ss >> x >> y >> z;
	return XMFLOAT3(x, y, z);
}

XMFLOAT3 ObjReader::ReadTextureAttributeLine(const char* attributeLine) {
	float u = 0.0f, v = 0.0f, w = 0.0f;
	std::stringstream ss(attributeLine);
	ss >> u >> v >> w;
	return XMFLOAT3(u, v, w);
}

std::vector<INT> ObjReader::ReadFaceAttributeLine(const char* attributeLine) {
	std::vector<INT> face;

	std::string vertex;
	std::stringstream vstream(attributeLine);
	while (vstream >> vertex) {
		std::string idx;
		std::stringstream istream(vertex);
		for (int i = 0; i < 3; i++) {
			if (std::getline(istream, idx, '/')) {
				INT fidx = static_cast<INT>(std::strtol(idx.c_str(), nullptr, 10));
				face.push_back(fidx - 1);
			} else {
				face.push_back(-1);
			}
		}
	}
	return face;
}

void ObjReader::ReadAttributeLine(int attributeId, const char* attributeLine) {
	switch (attributeId) {
		case 102: // f
			mF.push_back(ReadFaceAttributeLine(attributeLine));
			mVertexAmount += mF.back().size() - 6;
			break;
		case 118: // v
			mV.push_back(ReadVertexAttributeLine(attributeLine));
			break;
		case 228: // vn
			mVn.push_back(ReadNormalAttributeLine(attributeLine));
			break;
		case 234: // vt
			mVt.push_back(ReadTextureAttributeLine(attributeLine));
			break;
	}
}

void ObjReader::ReadFile(const char* filename) {
	std::ifstream in(filename);
	if (!in.is_open()) {
		throw EngineException(L"Unable to open the file.");
	}

	while (!in.eof()) {
		std::string attribute;
		std::string attributeLine;

		in >> attribute;
		std::getline(in, attributeLine);
		if (!attributeLine.empty()) {
			ReadAttributeLine(attribute[0] + attribute[1], attributeLine.c_str());
		}
	}
	in.close();
}

void ObjReader::CreateModelVertexBuffer(MODELBUFFER* model) {
	model->vertexBufferSize = mV.size();
	model->vertexBuffer = new VERTEX[model->vertexBufferSize];
	for (int i = 0; i < mF.size(); i++) {
		for (int j = 0; j < mF[i].size(); j += 3) {
			size_t positionIdx = mF[i][j];
			size_t textureIdx = mF[i][j + 1];
			size_t normalIdx = mF[i][j + 2];

			VERTEX vertex;
			vertex.position = mV[positionIdx];
			vertex.texcoord = textureIdx == -1 ? XMFLOAT3( 0.0, 0.0, 0.0 ) : mVt[textureIdx];
			vertex.normal = normalIdx == -1 ? XMFLOAT3( 0.0, 0.0, 0.0 ) : mVn[normalIdx];
			model->vertexBuffer[positionIdx] = vertex;
		}
	}
}

void ObjReader::CreateModelIndexBuffer(MODELBUFFER* model) {
	model->indexBufferSize = mVertexAmount;
	model->indexBuffer = new UINT[model->indexBufferSize];
	size_t currElemIdx = 0;
	for (size_t i = 0; i < mF.size(); i++) {
		//UINT firstVertex = static_cast<UINT>(mF[i][0]);
		//UINT secondVertex = static_cast<UINT>(mF[i][3]);
		//for (int j = 6; j < mF[i].size(); j += 3, currElemIdx += 3) {
		//	model->indexBuffer[currElemIdx + 2] = firstVertex;
		//	model->indexBuffer[currElemIdx] = secondVertex;
		//	model->indexBuffer[currElemIdx + 1] = mF[i][j];
		//	secondVertex = mF[i][j];
		//}
		UINT* sib = &(model->indexBuffer[currElemIdx]);
		sib[0] = static_cast<UINT>(mF[i][0]);
		sib[1] = static_cast<UINT>(mF[i][3]);
		sib[2] = static_cast<UINT>(mF[i][6]);
		currElemIdx += 3;

		UINT* startidx = sib;
		UINT* finalidx = sib + 2;
		for (size_t j = 9; j < mF[i].size(); j += 3, currElemIdx += 3) {
			UINT* sib = &(model->indexBuffer[currElemIdx]);
			sib[0] = *startidx;
			sib[1] = *finalidx;
			sib[2] = mF[i][j];

			startidx++;
			finalidx++;
		}
	}
}

MODELBUFFER ObjReader::GetModel() {
	MODELBUFFER model;
	CreateModelVertexBuffer(&model);
	CreateModelIndexBuffer(&model);
	return model;
}

void ObjReader::SaveLogFile(const char* filename) {
	std::ofstream out(filename);
	for (XMFLOAT4 pos : mV) {
		out << "v " << pos.x << " " << pos.y << " " << pos.z << " " << pos.w << "\n";
	}
	for (XMFLOAT3 nor : mVn) {
		out << "vn " << nor.x << " " << nor.y << " " << nor.z << "\n";
	}
	for (XMFLOAT3 tex : mVt) {
		out << "vt " << tex.x << " " << tex.y << " " << tex.z << "\n";
	}
	for (std::vector<INT> face : mF) {
		out << "f ";
		for (INT ver : face) {
			out << ver << "/";
		}
		out << "\n";
	}
	out.close();
}