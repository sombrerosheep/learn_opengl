#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct PointLight {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

struct DirectionalLight {
  vec3 	     direction;

  vec3 	     ambient;
  vec3 	     diffuse;
  vec3 	     specular;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform PointLight pointLights[2];
uniform DirectionalLight dLight;
uniform vec3 viewPos;
uniform float shininess;

vec3 cDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(-light.direction);

  float diff = max(dot(normal, lightDir), 0.0);
  
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

  vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
  vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

  return (ambient + diffuse + specular);
}

vec3 cPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 lightDir = normalize(light.position);

  float diff = max(dot(normal, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance +
       		        light.quadratic * (distance * distance));

  vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
  vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  return (ambient + diffuse + specular);
}

void main() {
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);

  vec3 result = cDirLight(dLight, norm, viewDir);

  result += cPointLight(pointLights[0], norm, FragPos, viewDir);
  result += cPointLight(pointLights[1], norm, FragPos, viewDir);

  FragColor = vec4(result, 1.0);
}