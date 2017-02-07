#version 410
// Fragment shader

in vec3 vertcolour_out;

out vec4 fColor;

void main() {

  fColor = vec4(vertcolour_out, 1.0);

}
