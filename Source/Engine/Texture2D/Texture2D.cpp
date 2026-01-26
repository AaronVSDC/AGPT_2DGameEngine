#include "Texture2D.h"
#include <SDL3/SDL.h>
#include <stdexcept>
#include <iostream>

namespace Papyrus
{
#include <SDL3/SDL.h>

    static void ApplyPinkToAlpha(SDL_Surface* s)
    {
        if (!s) return;

        // Lock if needed (safe for all surfaces)
        if (SDL_MUSTLOCK(s)) {
            if (SDL_LockSurface(s) != 0) {
                return;
            }
        }

        Uint8* bytes = static_cast<Uint8*>(s->pixels);
        const int pitch = s->pitch;

        // We assume 4 bytes/pixel (RGBA8888) because we converted to that format.
        for (int y = 0; y < s->h; ++y)
        {
            Uint8* row = bytes + y * pitch;
            for (int x = 0; x < s->w; ++x)
            {
                Uint8* p = row + x * 4; // RGBA
                const Uint8 r = p[0];
                const Uint8 g = p[1];
                const Uint8 b = p[2];

                p[3] = (r == 255 && g == 0 && b == 255) ? 0 : 255;
            }
        }

        if (SDL_MUSTLOCK(s)) SDL_UnlockSurface(s);
    }


    Texture2D::Texture2D(const std::string& fullPath)
    {
        SDL_Surface* loaded = SDL_LoadBMP(fullPath.c_str());
        if (!loaded)
            throw std::runtime_error(std::string("SDL_LoadBMP failed: ") + SDL_GetError());

        // Convert to RGBA8888 for predictable upload format
        SDL_Surface* surf = SDL_ConvertSurface(loaded, SDL_PIXELFORMAT_RGBA8888);
        SDL_DestroySurface(loaded);

        if (!surf)
            throw std::runtime_error(std::string("SDL_ConvertSurfaceFormat failed: ") + SDL_GetError());

        m_Width = surf->w;
        m_Height = surf->h;

        ApplyPinkToAlpha(surf);

        glGenTextures(1, &m_TextureId);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        // Upload pixels to GPU
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA8,
            m_Width,
            m_Height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            surf->pixels
        );

        glBindTexture(GL_TEXTURE_2D, 0);
        SDL_DestroySurface(surf);

        if (m_TextureId == 0)
            throw std::runtime_error("OpenGL texture creation failed");
    }

    Texture2D::~Texture2D()
    {
        if (m_TextureId)
            glDeleteTextures(1, &m_TextureId);
        m_TextureId = 0;
    }
}
