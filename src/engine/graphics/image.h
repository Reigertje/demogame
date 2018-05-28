#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include "GL/glew.h"

#include "texture.h"

class Image : public Texture {

public:

	Image(const std::string& image_path);

	~Image();

	void load();

	void unload();

	void bind();

	std::string getImagePath() const;

private:

	std::string image_path_;

	GLuint glid_;

};

#endif