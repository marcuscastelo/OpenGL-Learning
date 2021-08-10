#pragma once

#include <GL/glew.h>

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

class Renderer
{
public:
    void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const 
    {
        vao.Bind();
        ibo.Bind();
        shader.Bind();
        glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, 0);
    }

    void Clear() const 
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }
private:
};