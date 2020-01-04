#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <shader.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normals;
  glm::vec2 TexCoords;
};

struct Texture {
  unsigned int id;
  string type;
  string path;
};

class Mesh {
public:
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  vector<Texture> textures;

  Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
  void Draw(Shader shader);
private:
  unsigned int VAO, VBO, EBO;
  void setupMesh();
};

#endif
