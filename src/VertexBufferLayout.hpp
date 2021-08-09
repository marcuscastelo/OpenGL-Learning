#pragma once

#include <GL/glew.h>

#include <vector>
#include <stdint.h>
#include <signal.h>

struct VertexBufferLayoutElement
{
    uint32_t type;
    uint32_t count;
    uint32_t normalized;

    static uint32_t GetSize(uint32_t type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return 4;
        case GL_BYTE:
            return 1;
        case GL_UNSIGNED_BYTE:
            return 1;
        case GL_SHORT:
            return 2;
        case GL_UNSIGNED_SHORT:
            return 2;
        case GL_INT:
            return 4;
        case GL_UNSIGNED_INT:
            return 4;
        default:
            raise(SIGTRAP);
        }
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferLayoutElement> m_Elements;
    uint32_t m_Stride;

public:
    VertexBufferLayout()
        : m_Stride(0)
    {
    }

    template <typename T>
    void Push(uint32_t count)
    {
    }

    inline const std::vector<VertexBufferLayoutElement> &GetElements() const { return m_Elements; }
    inline uint32_t GetStride() const { return m_Stride; }
};

template <>
void VertexBufferLayout::Push<float>(uint32_t count)
{
    m_Elements.push_back({
        GL_FLOAT,
        count,
        GL_FALSE,
    });
    m_Stride += VertexBufferLayoutElement::GetSize(GL_FLOAT) * count;
}

template <>
void VertexBufferLayout::Push<uint32_t>(uint32_t count)
{
    m_Elements.push_back({
        GL_UNSIGNED_INT,
        count,
        GL_FALSE,
    });
    m_Stride += sizeof(uint32_t) * count;
}
