#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include <string>
#include <box2d/math_functions.h>
#include <glad/glad.h>
#include "PapyrusAPI.h"

namespace Papyrus
{
    class PAPYRUS_API Texture2D final 
    {
    public:
        explicit Texture2D(const std::string& fullPath);
        ~Texture2D();

        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
        Texture2D(Texture2D&&) = delete;
        Texture2D& operator=(Texture2D&&) = delete;

        GLuint getGLTexture() const { return m_TextureId; }
        b2Vec2 getSize() const { return { (float)m_Width, (float)m_Height }; }

        int width() const { return m_Width; }
        int height() const { return m_Height; }

    private:
        GLuint m_TextureId = 0;
        int m_Width = 0;
        int m_Height = 0;
    };
}

#endif