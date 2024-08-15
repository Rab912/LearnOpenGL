#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

    void use();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string &name, float x, float y) const;
    void setVec2(const std::string &name, const glm::vec2 value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec3(const std::string &name, const glm::vec3 value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;
    void setVec4(const std::string &name, const glm::vec4 value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

private:
    unsigned int m_id;
};