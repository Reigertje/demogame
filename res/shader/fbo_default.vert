#version 130

in vec2 vertex_xy;
in vec2 vertex_uv;

out vec2 fragment_uv;

void main() {
  fragment_uv = vertex_uv;
  gl_Position =  vec4(vertex_xy, 0.0, 1.0);
}