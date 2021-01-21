#include "StringUtility.h"

#include <sstream>

std::vector<std::string> split(std::string str, char delim) {
	std::vector<std::string> result;
	std::string substring;

	std::stringstream ss(str);
	while (std::getline(ss, substring, delim)) {
		result.push_back(substring);
	}
	return result;
}