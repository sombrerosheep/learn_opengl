#version 330 core

out vec4 FragColor;

uniform vec3 oColor;

void main() {
  FragColor = vec4(oColor, 1.0);
}