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

using namespace std::string_literals;

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



    // --- Code related to vertex buffer ---

    // Vertex buffer is a set of vertices that are passed to the shader in the form of different attributes glued together.
    // That means a vertex buffer can be represented as folloiwng:
    // vertexBuffer = [ vertex1, vertex2, vertex3, vertex4, ... ]
    // where vertex1, vertex2, vertex3, vertex4 are the vertex data
    // vertex_i can contain any data, from just coordinates, to texture coordinates, normals, colors, etc.
    // but the data is all stored sequentially, without any structure.
    uint32_t vbo; // Stands for Vertex Buffer Object
    glGenBuffers(1, &vbo);

    // In this example, we will use a vertex buffer with 4 vertices.
    // Each vertex contains only it's position, so we will use a float array of size 8
    // ( 2 floats for position inside each vertex )
    std::array<float, 8> vertexData{
        -0.5f, -0.5f, // First vertex (x, y)
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f,
    };

    // Then we need to tell OpenGl that we want to use this vertex buffer.
    // GL_ARRAY_BUFFER means we are binding a vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Then, we tell OpenGl to copy the data from our array to the vertex buffer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size(), vertexData.data(), GL_STATIC_DRAW);

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