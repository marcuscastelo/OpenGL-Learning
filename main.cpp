#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

using namespace std::string_literals;

uint32_t compileShader(uint32_t type, const std::string &path)
{
    std::ifstream shaderSourceFile;
    shaderSourceFile.open(path);
    if (!shaderSourceFile)
        throw std::runtime_error("Could not open "s + ( type ==GL_VERTEX_SHADER ? "vertex" : "fragment" ) + " shader file: "s + path);

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
        char *message = (char*) alloca((len + 1) * sizeof(char));
        glGetShaderInfoLog(shaderId, len, &len, message);

        throw std::runtime_error("Could not compile "s + ( type ==GL_VERTEX_SHADER ? "vertex" : "fragment" ) + " shader: "s + path + ":\n"s + message);

        glDeleteShader(shaderId);
    }

    return shaderId;
}

uint32_t createShaderProgram()
{
    uint32_t program = glCreateProgram();
    uint32_t vertexShader = compileShader(GL_VERTEX_SHADER, "shaders/vertexShader.vert");
    uint32_t fragmentShader = compileShader(GL_FRAGMENT_SHADER, "shaders/fragmentShader.frag");

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    glValidateProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

int main()
{
    GLFWwindow *window;

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    std::cout << "OpenGL " << glGetString(GL_VERSION) << " GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    uint32_t vertexBuffer;
    glGenBuffers(1, &vertexBuffer);

    float positions[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.0f, 0.5f};

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 3, positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    auto shaderProgram = createShaderProgram();
    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}