#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
  vec3 fragPos = vec3(model * vec4(aPos, 1.0));
  gl_Position = projection * view * vec4(fragPos, 1.0);
  TexCoords = aTexCoords;
}