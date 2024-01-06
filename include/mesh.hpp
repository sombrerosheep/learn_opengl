#ifndef MESH_H
#define MESH_H

#include <vector>

#include <shader.hpp>
#include <texture.hpp>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace std;

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normals;
  glm::vec2 TexCoords;
};

class Mesh {
public:
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  vector<Texture> textures;

  Mesh(vector<Vertex> vertices, vector<unsigned int> indices,
       vector<Texture> textures);
  void Draw(Shader shader);

private:
  unsigned int VAO, VBO, EBO;
  void setupMesh();
};

#endif // MESH_H
