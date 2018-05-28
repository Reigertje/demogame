#include "utils.h"

#include <sys/time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>
#include <iostream>

static bool isDirectory(const std::string& path) {
	struct stat s; 
	if (stat(path.c_str(), &s) == 0) {
		if( s.st_mode & S_IFDIR ) {
			return true;
		}
	} else {
		std::cerr << "Error : could not find " << path << std::endl;
	}
	return false;
}

static bool isFile(const std::string& path) {
	struct stat s; 
	if (stat(path.c_str(), &s) == 0) {
		if( s.st_mode & S_IFREG ) {
			return true;
		}
	} else {
		std::cerr << "Error : could not find " << path << std::endl;
	}
	return false;
}

static std::string getFullPath(const std::string& path, const std::string& file) {
	if (path[path.size() - 1] == '/' || path[path.size() - 1] == '\\') return path + file; 
	return path + "/" + file;
}

long utils::getCurrentTimeMillis() {
	struct timeval val;
	gettimeofday(&val, NULL);
	return val.tv_sec * 1000 + val.tv_usec/1000 + 0.5; 
}

std::string utils::readFileToString(const std::string& path) {
	std::string result = "";
 	std::ifstream fileStream(path.c_str(), std::ios::in);
    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << path << ". File does not exist." << std::endl;
        return result;
    }
    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        result.append(line + "\n");
    }
    fileStream.close();
    return result;
}


std::vector<std::string> utils::getFilesInFolder(const std::string path, bool recursive) {
	struct stat s; 
	DIR * dir; 
	struct dirent * entry; 

	std::vector<std::string> subdirectories; 
	std::vector<std::string> result;
	if (isDirectory(path)) {
		if ((dir = opendir(path.c_str())) != NULL) {
		  	while ((entry = readdir(dir)) != NULL) {
		  		if (entry->d_name[0] == '.') continue; 
		  		std::string filename = getFullPath(path, std::string(entry->d_name)); 
		  		if (isDirectory(filename)) {
		  			subdirectories.push_back(filename);
		  		} else if (isFile(filename)) {
		  			result.push_back(filename);
		  		}
		  	}
		  	closedir(dir);
		  	if (recursive) {
		  		for (int i = 0; i < subdirectories.size(); ++i) {
		  			std::vector<std::string> subResult = getFilesInFolder(subdirectories[i], recursive);
		  			result.insert(result.end(), subResult.begin(), subResult.end());
		  		}
		  	}

		} else {
			std::cerr << "Error : could not open directory : " << path << std::endl; 
		}
	} else {
		std::cerr << "Error : " << path << " is not a directory." << std::endl;
	}
	return result;
}