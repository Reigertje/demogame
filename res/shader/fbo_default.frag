#version 130

uniform sampler2D diffuse_sampler;

in vec2 fragment_uv;

out vec4 color_out;

void main() {
	color_out = texture2D(diffuse_sampler, fragment_uv);
}
