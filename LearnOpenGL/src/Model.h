#pragma once

#include <string>
#include <vector>

#include <assimp/scene.h>
#include <stb_image/stb_image.h>

#include "Shader.h"
#include "Mesh.h"

class Model
{
public:
    Model(const char* path);

    void draw(const Shader& shader);

private:
    void loadModel(std::string path);
    std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);

    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

private:
    std::vector<Mesh> m_meshes;
    std::vector<Texture> m_loadedTextures;
    std::string m_directory;
};