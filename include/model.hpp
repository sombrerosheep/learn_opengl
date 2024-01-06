#ifndef MODEL_H
#define MODEL_H

#include <mesh.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Model {
  public:
    Model(char *path) {
        loadModel(path);
    }

    void Draw(Shader shader);

  private:
    vector<Texture> textures_loaded;
    vector<Mesh>    meshes;
    string          directory;

    void            loadModel(string path);
    void            processNode(aiNode *node, const aiScene *scene);
    Mesh            processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};

#endif
