#version 130

uniform sampler2D diffuse_sampler;

uniform vec2 light_xy;
uniform vec4 light_rgba;

in vec2 fragment_uv;

out vec4 color_out;

const float sampleDist = 1.0;
const float sampleStrength = 1.0; 
const float dpx = 1.0/320.0;

const int num_samples = 100;
const float density = 0.5;
const float decay = 0.95;
const float weight = 0.5;
const float exposure = 0.10;

void main(void) {
  float N = float(num_samples);
  vec2 texCoord = fragment_uv;
  // Calculate vector from pixel to light source in screen space.

  vec2 deltaTexCoord = vec2(fragment_uv - light_xy);  

  // Divide by number of samples and scale by control factor.  
  //deltaTexCoord = deltaTexCoord * (1.0 / num_samples * density);  
  deltaTexCoord = deltaTexCoord * (1.0 / N * density);
  // Store initial sample.  
  vec4 color = texture2D(diffuse_sampler, texCoord);  

  // Set up illumination decay factor.  
  float illuminationdecay = 1.0;  

  // Evaluate summation from Equation 3 NUM_SAMPLES iterations.  
   for (int i = 0; i < num_samples; i++)  
  {  
    // Step sample location along ray.  
    texCoord -= deltaTexCoord;  
    // Retrieve sample at new location.  
    vec4 sample = texture2D(diffuse_sampler, texCoord); 
    // Apply sample attenuation scale/decay factors.  
    sample *= illuminationdecay * weight; 
    // Accumulate combined color.  
    color += sample;  
    // Update exponential decay factor.  
    illuminationdecay *= decay;  
  }  
  // Output final color with a further scale control factor.  
  //color.a = 1.0; 
  color_out = color * exposure;
  //gl_FragColor.a = 1.0; 
  //gl_FragColor =  texture2D(texture, fragment_texture_coord); 
  color_out.a = 1.0; 


  color_out = light_rgba * color_out;
  //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
