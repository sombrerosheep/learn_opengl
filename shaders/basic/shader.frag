#version 330 core

in vec3 oColor;

out vec4 FragColor;

uniform vec3 objectColor;

void main() {
  FragColor = vec4(oColor * objectColor, 1.0);
}