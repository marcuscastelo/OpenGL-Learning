#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <array>

#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"

using namespace std::string_literals;

#ifdef __WIN32__
#define DBG_BREAK() __debugbreak()
#else
#include <signal.h>
#define DBG_BREAK() raise(SIGTRAP)
#endif

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

void onError(int error, const char *description)
{
    std::cerr << "OpenGL Error: (" << error << ") " << description << std::endl;
}

void onOpenGLMessage(GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar *message,
                     const void *userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);

    if (GL_DEBUG_TYPE_ERROR == type)
        DBG_BREAK();
}

int main()
{
    GLFWwindow *window;

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }
    glfwSetErrorCallback(onError);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(onOpenGLMessage, NULL);

    std::cout << "OpenGL " << glGetString(GL_VERSION) << " GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    
    // --- Code related to vertex array object ---

    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // --- 


    // --- Code related to vertex buffer ---


    // In this example, we will use a vertex buffer with 4 vertices.
    // Each vertex contains only it's position, so we will use a float array of size 8
    // ( 2 floats for position inside each vertex )
    std::array<float, 8> vertexData{
        -0.5f,
        -0.5f, // First vertex (x, y)
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
    };

    VertexBuffer vbo(vertexData.data(), vertexData.size() * sizeof(float), GL_STATIC_DRAW);

    // Then we need to tell OpenGl that we want to use this vertex buffer.
    // GL_ARRAY_BUFFER means we are binding a vertex buffer.
    vbo.Bind();

    // As said before, all the data is sequential, so we need to specify two things:
    // 1 - What are the attributes of the vertex buffer (i.e. what values are stored in each vertex)
    // 2 - What is the stride between two vertices (i.e. how many bytes are between two vertices)

    // To achieve this, we specify the size of each attribute.
    // In this case, the first attribute (index 0) is the position, and it has 2 floats (2, GL_FLOAT)
    // After that, we specify the stride between two vertices, which is just the size of the position
    // (because we only have one attribute).
    // GL_FALSE means that the data does not need to be normalized.
    // (we are already using positions from -1 to 1, so no need to normalize)
    // Lastly, we specify the offset of the first vertex, which is 0.
    //
    // Great stackoverflow answer about the pointer parameter:
    // https://stackoverflow.com/questions/16380005/opengl-3-4-glvertexattribpointer-stride-and-offset-miscalculation
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    // ---

    // --- Code related to index buffer ---

    // Index buffer is a set of indices that are used
    // to avoid the need of duplicating the vertices.
    // In this example, we draw a square, so we need 4 vertices.
    // But we can draw only triangles, so we would need 6 vertices. (2 triangles)
    // Instead of duplicating the vertices, we can use the indices to specify
    // which vertices to draw in each triangle.

    uint32_t ibo; // Stands for Index Buffer Object
    glGenBuffers(1, &ibo);

    // The triangles are drawn in the counter-clockwise order.
    std::array<uint32_t, 6> indices{
        0, 1, 2, // First triangle
        2, 3, 0  // Second triangle
    };

    // GL_ELEMENT_ARRAY_BUFFER means we are binding an index buffer.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // Then, we tell OpenGl to copy the data from our array to the index buffer.
    // The same as before, we just copy the data from our array to the index buffer.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Now there's no need to call glEnableVertexAttribArray(0) anymore, because this
    // buffer is just a set of indices and does not contain any vertex data.
    // Thus, there are no attributes to enable.

    // ---

    // --- Code related to shader program ---

    auto shaderProgram = createShaderProgram();
    glUseProgram(shaderProgram);

    // ---

    uint32_t u_ColorLoc = glGetUniformLocation(shaderProgram, "u_Color");
    float r = 1.0f;
    float g = 0.6125f;
    float b = 0.25f;
    float dr = 0.01f;
    float dg = 0.01f;
    float db = 0.01f;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform4f(u_ColorLoc, r, g, b, 1.0f);

        // Note: the vbo and ibo are already bound to opengl
        // so all the operations below refer to them.

        // Deactivated: used to draw vertex without an index buffer
        // glDrawArrays(GL_TRIANGLES, 0, 6);

        // Using a index buffer:
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        r += dr;
        g += dg;
        b += db;

        if (r > 1.0f || r < 0.25f)
            dr *= -1.0f;

        if (g > 1.0f || g < 0.25f)
            dg *= -1.0f;

        if (b > 1.0f || b < 0.25f)
            db *= -1.0f;

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}