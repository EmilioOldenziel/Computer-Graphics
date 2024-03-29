#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the inputs to the fragment shader
// These must have the same type and name!

in vec4 position_pixel;
in vec3 normal;
in vec2 tex_coors;

// in vec3 vertPos; Using the output from the vertex shader example

// Specify the Uniforms of the vertex shaders
// uniform vec3 lightPosition; for example

uniform vec3 colour_object_in;
uniform vec3 colour_light_in;
uniform vec4 material_in;
uniform vec3 position_light_in;
uniform sampler2D sampler;

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
layout (location = 0) out vec3 fColor;
layout (location = 1) out vec3 normal_out;

void main()
{
    // Set colour to ambient lighting.
    // vec3 ambient = colour_object_in * material_in[0];

    // // Determine directions of light, view and refraction.
    // vec3 direction_light = normalize (position_light_in - position_pixel.xyz);
    // vec3 direction_view = normalize (-(position_pixel.xyz));
    // vec3 direction_refraction = normalize(2 * dot (normal, direction_light) * normal - direction_light);

    // // Add diffuse light.
    // vec3 diffuse = colour_object_in * max (0.0, dot (normal, direction_light) * material_in[1]);

    // // Add specular light.
    // float specular = material_in[2] * pow (max (0.0, dot (direction_refraction, direction_view)), material_in[3]);

    // fColor = vec4(ambient + diffuse + specular, 1.0);

    fColor = texture(sampler, tex_coors).rgb;
    normal_out = (normal+1)/2.0;

    //fColor = vec4(1.0,1.0,0,1.0);

    // -------------------------------------------------------------------------
    // This is normal buffer. Uncomment this line and comment the rest for 
    // normal buffer rendering. Debugging purposes.

    //fColor = vec4((normal+1)/2, 1.0);
}
