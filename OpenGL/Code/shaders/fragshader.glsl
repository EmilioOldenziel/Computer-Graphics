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

in vec3 colour_object;
in vec3 colour_light;
in vec4 material;
in vec3 position_light;

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;


void main()
{
        // Set colour to ambient lighting.
    vec3 ambient, diffuse;
    ambient = colour_object * material[0];

    // Determine directions of light, view and refraction.
    vec3 direction_light = normalize (position_light - position_pixel.xyz);
    vec3 direction_view = normalize (-(position_pixel.xyz));
    vec3 direction_refraction = normalize(2 * dot (normal, direction_light) * normal - direction_light);

    // Add diffuse light.
    diffuse = colour_object * max (0.0, dot (normal, direction_light) * material[1]);

    // Add specular light.
    float specular = material[2] * pow (max (0.0, dot (direction_refraction, direction_view)), material[3]);



    //fColor = vec4(ambient + diffuse + specular, 1.0);
    fColor = vec4((normal+1)/2, 1.0);
}
