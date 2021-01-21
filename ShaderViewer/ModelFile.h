#ifndef _MODEL_FILE_H_
#define _MODEL_FILE_H_

#include "ModelFileCommon.h"
#include <list>

class ModelFile {
public:
	ModelFile(const char* filename);

	const char* Filename() const;
private:
	void ReadExternalFile();

	const char* mFilename;
	std::list<Vertex> mVList;
	std::list<VertexNormal> mVNList;
	std::list<Face> mFList;
};

#endif // !_FILE_H_
