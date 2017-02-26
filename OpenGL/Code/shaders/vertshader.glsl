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
// uniform vec3 colour;
// uniform vec4 material;
// uniform vec3 light;

// Specify the outputs of the vertex shader
// These will be the input for the fragment shader

// out vec3 vertex_colour;
// out vec4 vertex_material;
// out vec3 vertex_light;
out vec4 position_vertex;
out vec3 normal;

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    // Currently without any transformation
    vec4 pos = projection * view * model * vec4(vertCoordinates_in, 1.0);

    gl_Position = pos;

    position_vertex = pos;
    normal = vertNormal_in;
}
