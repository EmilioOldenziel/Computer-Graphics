#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 2) in vec3 vertNormal_in;

// Specify the Uniforms of the vertex shader

// uniform mat4 modelTransform; for example
uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
uniform vec3 colour_object_in;
uniform vec3 colour_light_in;
uniform vec4 material_in;
uniform vec3 position_light_in;
uniform mat3 normal_matrix;

// Specify the outputs of the vertex shader
// These will be the input for the fragment shader

out vec3 colour_object;
out vec3 colour_light;
out vec4 material;
out vec3 position_light;
out vec4 position_pixel;
out vec3 normal;

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    // Currently without any transformation
    vec4 pos = view * model * vec4(vertCoordinates_in, 1.0);

    gl_Position = projection * pos;

    position_pixel = pos;

    colour_object = colour_object_in;
    colour_light = colour_light_in;
    material = material_in;
    position_light = position_light_in;

    normal = normalize (normal_matrix * vertNormal_in);
}
