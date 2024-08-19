#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Model.h"

unsigned int loadTextureFromFile(const char* path, const std::string& directory)
{
    std::string filename = directory + '/' + std::string(path);

    unsigned int id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format;
        switch (nrChannels)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            format = GL_RGBA;
            break;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return 0;
    }

    stbi_image_free(data);
    return id;
}

Model::Model(const char* path)
{
    loadModel(path);
}

void Model::draw(const Shader& shader)
{
    for (int i = 0; i < m_meshes.size(); i++)
        m_meshes[i].draw(shader);
}

void Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "Failed to load scene" << std::endl;
        return;
    }

    m_directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString path;
        material->GetTexture(type, i, &path);

        bool skip = false;
        for (int j = 0; j < m_loadedTextures.size(); j++)
        {
            if (std::strcmp(m_loadedTextures[j].path.data(), path.C_Str()) == 0)
            {
                textures.push_back(m_loadedTextures[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            Texture texture;
            texture.id = loadTextureFromFile(path.C_Str(), m_directory);
            texture.type = typeName;
            texture.path = path.C_Str();
            textures.push_back(texture);
            m_loadedTextures.push_back(texture);
        }
    }

    return textures;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // Process each mesh in node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }

    // Do the same for each of the node's children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        // store position, normal, and texture coordinates
        vertex.position = glm::vec3(mesh->mVertices[i].x,
                                    mesh->mVertices[i].y,
                                    mesh->mVertices[i].z);

        vertex.normal = glm::vec3(mesh->mNormals[i].x,
                                  mesh->mNormals[i].y,
                                  mesh->mNormals[i].z);

        if (mesh->mTextureCoords[0])
            vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x,
                                         mesh->mTextureCoords[0][i].y);
        else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // indices, material
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
                                                                aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = loadMaterialTextures(material,
                                                                aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}
