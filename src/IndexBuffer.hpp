#pragma once

#include <stdint.h>
#include <GL/glew.h>

// Index buffer is a set of indices that are used
// to avoid the need of duplicating the vertices.
// In this example, we draw a square, so we need 4 vertices.
// But we can draw only triangles, so we would need 6 vertices. (2 triangles)
// Instead of duplicating the vertices, we can use the indices to specify
// which vertices to draw in each triangle.
class IndexBuffer
{
public:
    IndexBuffer(uint32_t *data, uint32_t count, uint32_t usage)
        : m_Count(count)
    {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, usage);
    }

    ~IndexBuffer(void)
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void Bind(void) const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void Unbind(void) const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    inline uint32_t GetCount(void) const { return m_Count; }

private:
    uint32_t m_RendererID;
    uint32_t m_Count;
};