#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Shader.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void draw(const Shader& shader);

private:
    void setup();

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;

    unsigned int m_vao, m_vbo, m_ebo;
};