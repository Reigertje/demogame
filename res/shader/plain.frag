#version 130

uniform sampler2D diffuse_sampler;

in vec2 fragment_uv;

uniform vec4 multiply_color;
uniform vec4 mix_color;

out vec4 color_out;

void main() {
	vec4 diffuse_sample = texture2D(diffuse_sampler, fragment_uv);
	color_out.rgb = multiply_color.rgb * mix(diffuse_sample.rgb, mix_color.rgb, mix_color.a);
	color_out.a = multiply_color.a * diffuse_sample.a;
}
