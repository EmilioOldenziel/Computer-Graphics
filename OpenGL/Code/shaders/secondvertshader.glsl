#version 330 core

// Specify the input locations of attributes
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 2) in vec3 vertNormal_in;
layout (location = 3) in vec2 textureCoordinates_in;

// uniform mat4 modelTransform; for example
uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

uniform mat3 normal_matrix;

out vec4 position_pixel;
out vec3 normal;
out vec2 tex_coors;

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    // Currently without any transformation
    vec4 pos = view * model * vec4(vertCoordinates_in, 1.0);

    gl_Position = projection * pos;

    position_pixel = pos;

    normal = normalize (normal_matrix * vertNormal_in);
    tex_coors = textureCoordinates_in;
}
