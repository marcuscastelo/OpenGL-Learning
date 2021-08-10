#pragma once

#include <GL/glew.h>

#include <stdint.h>
#include <string>

#include "vendor/stb_image/stb_image.h"

class Texture
{
private:
    uint32_t m_RendererID;
    std::string m_Filepath;
    uint8_t *m_LocalBuffer;
    int m_Width, m_Height, m_BPP;

public:

    Texture(const std::string &filepath) 
        : m_Filepath(filepath), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
    {
        stbi_set_flip_vertically_on_load(true);
        m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (m_LocalBuffer) {
            stbi_image_free(m_LocalBuffer);
            m_LocalBuffer = nullptr;
        }
    }

    ~Texture() 
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void Bind(uint32_t slot = 0) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }

    void Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
    inline int GetBPP() const { return m_BPP; }
};
