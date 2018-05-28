#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace utils {

	long getCurrentTimeMillis();

	std::string readFileToString(const std::string& path);

	std::vector<std::string> getFilesInFolder(const std::string path, bool recursive);
}


#endif