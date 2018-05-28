#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#ifdef _WIN32
	#include <winsock.h>
#else 
	#include "arpa/inet.h"
#endif

Image::Image(const std::string& image_path) {
	image_path_ = image_path;

	glid_ = 0; 
	
	setP0(glm::vec2(0.0f, 1.0f));
	setP1(glm::vec2(1.0f, 0.0f));

	// Read dimensions from png file

	std::ifstream in(image_path_.c_str());
    unsigned int width = 0;
    unsigned int height = 0; 

    in.seekg(16);
    in.read((char *)&width, 4);
    in.read((char *)&height, 4);

    setPixelWidth(ntohl(width));
    setPixelHeight(ntohl(height));
}

Image::~Image() {
	unload();
}

void Image::load() {
	if (!glid_) {
		std::vector<unsigned char> imageBuffer; 

		int w, h, comp; 
		GLuint t;

		unsigned char * im = stbi_load(image_path_.c_str(), &w, &h, &comp, 0);

		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &t); 
		glBindTexture(GL_TEXTURE_2D, t);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, im); 

		glBindTexture(GL_TEXTURE_2D, 0);

		free(im);

		glid_ = t;
	}
}

void Image::unload() {
	if (!glid_) return;
	glDeleteTextures(1, &glid_);
}

void Image::bind() {
	if (glid_ == -1) {
		std::cerr << "Error : image to bind is not loaded" << std::endl;  
		exit(-1); 
	}
	glBindTexture(GL_TEXTURE_2D, glid_); 
}

std::string Image::getImagePath() const {
	return image_path_;
}