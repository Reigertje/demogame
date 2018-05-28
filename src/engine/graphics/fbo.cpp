#include "fbo.h"

#include "shader.h"

#include <iostream>

static const float vertices[] = {
	-1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, 1.0f,
	-1.0f, 1.0f
};

static const float texture_coordinates[] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f
};


static const float flipped_texture_coordinates[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f
};


FBO::FBO(int width, int height, bool linear) {
	width_ = width;
	height_ = height;

	glGenFramebuffers(1, &fbo_);
	glGenTextures(1, &tbo_);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

	glBindTexture(GL_TEXTURE_2D, tbo_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tbo_, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	checkStatus();
}

FBO::~FBO() {
	if (fbo_ >= 0) glDeleteBuffers(1, &fbo_); 
}

GLuint FBO::getFboId() const {
	return fbo_;
}

int FBO::getWidth() const {
	return width_;
}

int FBO::getHeight() const {
	return height_;
}


void FBO::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
	glViewport(0, 0, width_, height_);
}

void FBO::bindScreen(int screen_width, int screen_height) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, screen_width, screen_height);
}

void FBO::draw(Shader& shader, bool mirror_vertical) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tbo_);
	glUniform1i(shader.getUniformHandler("diffuse_sampler"), 0);

	glVertexAttribPointer(shader.getAttribHandler("vertex_xy"), 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(shader.getAttribHandler("vertex_uv"), 2, GL_FLOAT, GL_FALSE, 0, mirror_vertical ? flipped_texture_coordinates : texture_coordinates);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

bool FBO::checkStatus() {
	GLenum status;
	status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER); 
	switch(status) {
		case GL_FRAMEBUFFER_COMPLETE:
			return true;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cerr << "Framebuffer not supported" << std::endl;
			return false;
			break;
		default:
			/* programming error; will fail on all hardware */
			std::cerr << "Framebuffer failed due to software" << std::endl;
			exit(-1);
	}
}