#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;
    
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
    {
        // === Shader code extraction ===
        
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        
        try
        {
            vShaderFile.open(vertexShaderPath);
            fShaderFile.open(fragmentShaderPath);

            std::stringstream vShaderStream, fShaderStream;
            
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "Shader file(s) not successfully read" << std::endl;
            return;
        }
        
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];
             
        // === Vertex shader creation ===
        
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
            std::cout << "Vertex shader compilation failed\n" << infoLog << std::endl;
            return;
        }
        
        // === Fragment shader creation ===
        
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
            std::cout << "Vertex shader compilation failed\n" << infoLog << std::endl;
            return;
        }
        
        // === Linking the shaders together ===
        
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, nullptr, infoLog);
            std::cout << "Shader linking failed\n" << infoLog << std::endl;
            return;
        }
        
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    
    void use()
    {
        glUseProgram(ID);
    }
    
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
};