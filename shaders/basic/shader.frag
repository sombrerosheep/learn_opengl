#version 330 core

struct Material {
  sampler2D    diffuse;
  sampler2D    specular;

  float	     shininess;
};

struct DirectionalLight {
  vec3 	     direction;

  vec3 	     ambient;
  vec3 	     diffuse;
  vec3 	     specular;
};

struct PointLight {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

struct SpotLight {
  vec3 position;
  vec3 direction;
  float cutOff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};  

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform SpotLight light;

void main() {
  vec3 lightDir = normalize(light.position - FragPos);

  float theta = dot(lightDir, normalize(-light.direction));

  if (theta > light.cutOff) { 
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
   
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
  } else {
    FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
  }
}