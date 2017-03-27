#version 330 core

in vec4 position_pixel;
in vec3 normal;
in vec2 tex_coors;

uniform sampler2D sampler;

layout (location = 0) out vec3 colour;
layout (location = 1) out vec3 normals;

void main () {
	colour = texture2D(sampler, tex_coors).rgb; 
	normals = (normal + 1) / 2;
}
