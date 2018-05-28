#version 130

uniform mat4 m;
uniform mat4 mvp;

in vec2 vertex_xy;
in vec2 vertex_uv;
in vec2 vertex_n_uv;

out vec2 fragment_xy;
out vec2 fragment_uv;
out vec2 fragment_n_uv;

void main() {
	fragment_uv = vertex_uv;
	fragment_n_uv = vertex_n_uv;

	fragment_xy = (m * vec4(vertex_xy, 0.0, 1.0)).xy;

	gl_Position =  mvp * vec4(vertex_xy, 0.0,  1.0);
}