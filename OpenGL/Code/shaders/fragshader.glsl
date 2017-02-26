#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the inputs to the fragment shader
// These must have the same type and name!

in vec4 position_pixel;
in vec3 normal;

// in vec3 vertPos; Using the output from the vertex shader example

// Specify the Uniforms of the vertex shaders
// uniform vec3 lightPosition; for example

uniform vec3 colour_object;
uniform vec3 colour_light;
uniform vec4 material;
uniform vec3 position_light;

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;


void main()
{
	// Set colour to ambient lighting.
    fColor = vec4(colour_object * material[0], 1.0);

    // Determine directions of light, view and refraction.
    vec3 direction_light = normalize (position_light - vec3 (position_pixel));
    vec3 direction_view = normalize (- vec3 (position_pixel));
    vec3 direction_refraction = 2 * dot (direction_light, normal) * normal - direction_light;

    // Add diffuse light.
    fColor += vec4(colour_object, 1.0) * max (0.0, dot (normal, direction_light) * material[1]);

    // Add specular light.
    fColor += vec4(colour_object, 1.0) * material[2] * 
    	pow (max (0.0, dot (direction_refraction, direction_view)), material[3]);

    // fColor = vec4(normal, 1.0);
}
