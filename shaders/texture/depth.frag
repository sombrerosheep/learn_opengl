#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D tex;

float near = 0.1;
float far = 100.0;

void main() {
  float z = gl_FragCoord.z * 2.0 - 1.0;
  float linearDepth = (2.0 * near * far) / (far + near - z * (far - near));
  
  FragColor = vec4(vec3(linearDepth / far), TexCoords);
}