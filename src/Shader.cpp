#include <GL/glew.h>

#include "Shader.hpp"

#include <stdint.h>
#include <string>
#include <fstream>
#include <sstream>

uint32_t Shader::CompileShader(uint32_t type, const std::string &path)
{
    std::ifstream shaderSourceFile;
    shaderSourceFile.open(path);
    if (!shaderSourceFile)
        throw std::runtime_error("Could not open "s + (type == GL_VERTEX_SHADER ? "vertex" : "fragment") + " shader file: "s + path);

    std::stringstream ss;
    while (ss << shaderSourceFile.rdbuf())
        ;
    std::string shaderSource = ss.str();

    uint32_t shaderId = glCreateShader(type);
    const char *source = shaderSource.c_str();
    glShaderSource(shaderId, 1, &source, nullptr);
    glCompileShader(shaderId);

    int result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int len;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &len);
        char *message = (char *)alloca((len + 1) * sizeof(char));
        glGetShaderInfoLog(shaderId, len, &len, message);

        throw std::runtime_error("Could not compile "s + (type == GL_VERTEX_SHADER ? "vertex" : "fragment") + " shader: "s + path + ":\n"s + message);

        glDeleteShader(shaderId);
    }

    return shaderId;
}