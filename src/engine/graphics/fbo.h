#ifndef FBO_H
#define FBO_H

#include "GL/glew.h"

class Shader;

class FBO {

public:

	FBO(int width, int height, bool linear = false);

	~FBO();

	void draw(Shader& shader, bool mirror_vertical);

	GLuint getFboId() const;

	int getWidth() const;

	int getHeight() const;

	void bind();

	static void bindScreen(int screen_width, int screen_height);

private:

	bool checkStatus();

	GLuint fbo_;

	GLuint tbo_;

	int width_; 

	int height_;

};

#endif