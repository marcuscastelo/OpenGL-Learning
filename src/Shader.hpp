#pragma once

#include <GL/glew.h>

#include <string>
#include <iostream>

using namespace std::string_literals;

class Shader
{
public:
    Shader(const std::string &vertexFilePath, const std::string &fragmentFilePath)
    {
        uint32_t vertexShaderID = CompileShader(GL_VERTEX_SHADER, vertexFilePath);
        uint32_t fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentFilePath);

        m_RendererID = glCreateProgram();
        glAttachShader(m_RendererID, vertexShaderID);
        glAttachShader(m_RendererID, fragmentShaderID);
        glLinkProgram(m_RendererID);

        glValidateProgram(m_RendererID);

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }

    ~Shader()
    {
        glDeleteProgram(m_RendererID);
    }

    void Bind() const { glUseProgram(m_RendererID); }
    void Unbind() const { glUseProgram(0); }

    uint32_t GetUniformLocation(const std::string &uniformName) const
    {
        int32_t location = glGetUniformLocation(m_RendererID, uniformName.c_str());
        if (location == -1)
        {   
            std::cout << "Uniform \"" << uniformName << "\" not found in shader" << std::endl;
        }
        return location;
    }

    template <typename ...T>
    void SetUniform(uint32_t location, T ...values) const {
        std::cerr << "Not implemented" << std::endl;
    }

    template <typename ...T>
    void SetUniform(const std::string &uniformName, T ...values) const {
        SetUniform(GetUniformLocation(uniformName), values...);
    }

private:
    uint32_t CompileShader(uint32_t type, const std::string &path);

    uint32_t m_RendererID;
};