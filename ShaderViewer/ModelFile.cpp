#include "ModelFile.h"
#include "EngineException.h"

#include <string>
#include <sstream>
#include <fstream>

ModelFile::ModelFile(const char* filename) : mFilename(filename) {

}

const char* ModelFile::Filename() const {
	return mFilename;
}

void ModelFile::ReadExternalFile() {
	std::ifstream in(mFilename);

	if (!in.is_open()) {
		throw EngineException(L"ModelFile::ReadExternalFile() file not found.");
	}

	while (!in.eof()) {
		std::string line;
		std::getline(in, line);
	}

	in.close();
}