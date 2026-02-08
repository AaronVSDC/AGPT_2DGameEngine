#include "Texture2D.h"
#include <SDL3/SDL.h>
#include <stdexcept>
#include <vector>
namespace Papyrus
{

    Texture2D::Texture2D(const std::string& fullPath)
    {
        SDL_Surface* loaded = SDL_LoadBMP(fullPath.c_str());
        if (!loaded) throw std::runtime_error(SDL_GetError());

        SDL_Surface* surf = SDL_ConvertSurface(loaded, SDL_PIXELFORMAT_RGBA32);
        SDL_DestroySurface(loaded);
        if (!surf) throw std::runtime_error(SDL_GetError()); 

        m_Width = surf->w;
        m_Height = surf->h;

        if (SDL_MUSTLOCK(surf)) {
            if (SDL_LockSurface(surf) != 0) {
                SDL_DestroySurface(surf);
                throw std::runtime_error(SDL_GetError());
            }
        }

        std::vector<uint8_t> packed(m_Width * m_Height * 4);
        const uint8_t* src = static_cast<const uint8_t*>(surf->pixels);

        for (int y = 0; y < m_Height; ++y)
        {
            const uint8_t* rowSrc = src + y * surf->pitch;
            uint8_t* rowDst = packed.data() + y * (m_Width * 4);
            std::memcpy(rowDst, rowSrc, (size_t)m_Width * 4);
        }

        if (SDL_MUSTLOCK(surf)) SDL_UnlockSurface(surf);
        SDL_DestroySurface(surf);


        glGenTextures(1, &m_TextureId);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // packed RGBA has no padding issues

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, packed.data());

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    Texture2D::~Texture2D()
    {
        if (m_TextureId)
            glDeleteTextures(1, &m_TextureId);
        m_TextureId = 0;
    }
}
