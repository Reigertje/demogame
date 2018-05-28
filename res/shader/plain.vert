#version 130

uniform mat4 mvp;

in vec2 vertex_xy;
in vec2 vertex_uv;

out vec2 fragment_xy;

out vec2 fragment_uv;

void main() {
	fragment_uv = vertex_uv;
	gl_Position =  mvp * vec4(vertex_xy, 0.0,  1.0);
}