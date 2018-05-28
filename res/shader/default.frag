#version 130

#define MAX_LIGHTS 24

uniform mat4 m;
uniform int light_count;
uniform vec4 light_data[MAX_LIGHTS * 2];

uniform sampler2D diffuse_sampler;
uniform sampler2D normal_sampler;

uniform vec4 multiply_color;
uniform vec4 mix_color;

uniform vec3 ambient_light;

in vec2 fragment_xy;
in vec2 fragment_uv;
in vec2 fragment_n_uv;

out vec4 color_out;

vec3 desaturate(vec3 color, float amount)
{
    vec3 gray = vec3(dot(vec3(0.2126,0.7152,0.0722), color));
    return vec3(mix(color, gray, amount));
}

vec3 applyLight(int i, vec3 N) {
	vec3 light_position = light_data[i * 2].xyz;
	vec3 light_color = light_data[i * 2 + 1].xyz;
	float light_radius = light_data[i * 2 + 1].a;

	vec3 light_direction = light_position - vec3(fragment_xy, 0.0);
	float dist = length(light_direction);

	vec3 L = normalize(light_direction);
	vec3 diffuse = light_color * max(dot(N, L), 0.0);

	float attenuation  = clamp(1.0 - dist * dist /(light_radius * light_radius), 0.0, 1.0);

	return diffuse * attenuation * attenuation;
}

void main() {

	vec4 diffuse_sample = texture2D(diffuse_sampler, fragment_uv);
	vec4 normal_sample = texture2D(normal_sampler, fragment_n_uv);

	vec2 normal_xy = normal_sample.rg * 2.0 - 1.0;
	vec3 normal_xyz = vec3(normal_xy, sqrt(1.0 - normal_xy.x * normal_xy.x - normal_xy.y * normal_xy.y));

	mat3 TBN = mat3(
		normalize((m * vec4(1.0, 0.0, 0.0, 0.0)).xyz), // Tangent
		normalize((m * vec4(0.0, 1.0, 0.0, 0.0)).xyz), // Bitangent
		normalize((m * vec4(0.0, 0.0, -1.0, 0.0)).xyz) // Normal
	);

	vec3 N = normalize(TBN * normalize(normal_xyz));

	// HARDCODED AMBIENT
	vec3 intensity = ambient_light;
	//vec3 intensity = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < light_count; ++i) {
		intensity += applyLight(i, N);
	}

	//color_out = normal_sample;

/* 	fragment_vertex_color.rgb * mix(FinalColor, object_color.rgb, object_color.a);
  gl_FragColor.a = diffuseColor.a * fragment_vertex_color.a; */


	color_out.rgb = multiply_color.rgb * mix(diffuse_sample.rgb * intensity, mix_color.rgb, mix_color.a);
	color_out.a = multiply_color.a * diffuse_sample.a;
}