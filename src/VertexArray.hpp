#pragma once

#include <GL/glew.h>

#include <stdint.h>

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray
{
public:
    VertexArray() {
        glGenVertexArrays(1, &m_RendererID);
    }

    ~VertexArray() {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void Bind() {
        glBindVertexArray(m_RendererID);
    }

    void Unbind() {
        glBindVertexArray(0);
    }

    void AddVBO(const VertexBuffer& vbo, const VertexBufferLayout& layout) {
        this->Bind();
        vbo.Bind();

        const auto& elements = layout.GetElements();
        
        uint32_t offset = 0;
        for (size_t i = 0; i < elements.size(); ++i) {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, layout.GetStride(), (const void*) offset);
            offset += elements[i].count * VertexBufferLayoutElement::GetSize(elements[i].type);
        }
    }

private:
    uint32_t m_RendererID;
};