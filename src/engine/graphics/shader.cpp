#include "shader.h"

#include <string.h>
#include <cstdlib>
#include <iostream>
#include <vector>

Shader::Shader(const std::string& vertex_shader, const std::string& fragment_shader) {
	vertex_shader_ = vertex_shader;
	fragment_shader_ = fragment_shader;
	glid_ = -1;
}

void Shader::load() {

	// Compile shader
	if (glid_ == -1) {

		GLuint gl_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		GLuint gl_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

		const char *vertex_shader_c_str = vertex_shader_.c_str();
		const char *fragment_shader_c_str = fragment_shader_.c_str();

		GLint result = GL_FALSE;

		int logLength;

		// Compile vertex shader
		glShaderSource(gl_vertex_shader, 1, &vertex_shader_c_str, NULL);
		glCompileShader(gl_vertex_shader);

		// Check vertex shader
		glGetShaderiv(gl_vertex_shader, GL_COMPILE_STATUS, &result);
		glGetShaderiv(gl_vertex_shader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> vertex_shader_error(logLength);
		glGetShaderInfoLog(gl_vertex_shader, logLength, NULL, &vertex_shader_error[0]);

		if (vertex_shader_error.size() > 0 && strlen(&vertex_shader_error[0]) > 0) {
			std::cerr << &vertex_shader_error[0] << std::endl;
		}

		glShaderSource(gl_fragment_shader, 1, &fragment_shader_c_str, NULL);
		glCompileShader(gl_fragment_shader);

		// Check fragment shader
		glGetShaderiv(gl_fragment_shader, GL_COMPILE_STATUS, &result);
		glGetShaderiv(gl_fragment_shader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> fragment_shader_error(logLength);
		glGetShaderInfoLog(gl_fragment_shader, logLength, NULL, &fragment_shader_error[0]);

		if (fragment_shader_error.size() > 0 && strlen(&fragment_shader_error[0]) > 0) {
			std::cerr << &fragment_shader_error[0] << std::endl;
		}

		// Link
		GLuint program = glCreateProgram();
		glAttachShader(program, gl_vertex_shader);
		glAttachShader(program, gl_fragment_shader);
		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &result);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> programError( (logLength > 1) ? logLength : 1 );
		glGetProgramInfoLog(program, logLength, NULL, &programError[0]);

		if (programError.size() > 0 && strlen(&programError[0]) > 0) {
			std::cerr << &programError[0] << std::endl;
		}

		glDeleteShader(gl_vertex_shader);
		glDeleteShader(gl_fragment_shader);

		glid_ = program;
	}
}

void Shader::enableAttribArrays() {
	glEnableVertexAttribArray(getAttribHandler("vertex_xy"));
	glEnableVertexAttribArray(getAttribHandler("vertex_uv"));
	glEnableVertexAttribArray(getAttribHandler("vertex_n_uv"));
}

void Shader::bind() {
	if (glid_ != -1) {
		glUseProgram(glid_);
		enableAttribArrays();
	} else {
		std::cerr << "Error : uncompiled shader bound" << std::endl;
		exit(-1); 
	}
}

GLint Shader::getId() const {
	return glid_;
}

GLint Shader::getUniformHandler(const std::string& key) {
	// TODO store in handler map
	glGetUniformLocation(glid_, key.c_str());
}

GLint Shader::getAttribHandler(const std::string& key) {
	// TODO store in handler map
	glGetAttribLocation(glid_, key.c_str());
}

