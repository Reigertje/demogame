#ifndef SHADER_H
#define SHADER_H

#include "GL/glew.h"
#include <string>
#include <map>

class Shader {

public:

	Shader(const std::string& vertex_shader, const std::string& fragment_shader);

	void load();

	void bind();

	GLint getId() const;

	GLint getUniformHandler(const std::string& key);

	GLint getAttribHandler(const std::string& key);

private:

	void enableAttribArrays();

	GLint glid_;

	std::string vertex_shader_;

	std::string fragment_shader_;

	std::map<std::string, GLint> handlers_;

};

#endif