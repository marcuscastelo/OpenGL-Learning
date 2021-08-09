#pragma once

#include <stdint.h>
#include <GL/glew.h>

// Vertex buffer is a set of vertices that are passed to the shader in the form of different attributes glued together.
// That means a vertex buffer can be represented as folloiwng:
// vertexBuffer = [ vertex1, vertex2, vertex3, vertex4, ... ]
// where vertex1, vertex2, vertex3, vertex4 are the vertex data
// vertex_i can contain any data, from just coordinates, to texture coordinates, normals, colors, etc.
// but the data is all stored sequentially, without any structure.
class VertexBuffer
{
public:
    VertexBuffer(void *data, uint32_t size, uint32_t usage)
    {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    }

    void Bind(void) const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void Unbind(void) const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ~VertexBuffer(void)
    {
        glDeleteBuffers(1, &m_RendererID);
    }

private:
    uint32_t m_RendererID;
};