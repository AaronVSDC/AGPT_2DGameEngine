#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include "Texture2D.h"
#include "Renderer.h"
#include <stdexcept>
#include <cassert>
#include <iostream>

Papyrus::Texture2D::~Texture2D()
{
    if (m_texture)
        SDL_DestroyTexture(m_texture);
}

b2Vec2 Papyrus::Texture2D::getSize() const
{
    float w = 0.0f;
    float h = 0.0f;

    if (!SDL_GetTextureSize(m_texture, &w, &h))
        std::cerr << "SDL_GetTextureSize failed: " << SDL_GetError() << std::endl;

    return { w, h };
}

SDL_Texture* Papyrus::Texture2D::getSDLTexture() const
{
    return m_texture;
}

Papyrus::Texture2D::Texture2D(const std::string& fullPath)
{
    SDL_Renderer* renderer = Renderer::getInstance().getSDLRenderer();

    SDL_Surface* surface = SDL_LoadBMP(fullPath.c_str());
    if (!surface)
        std::cerr << "SDL_LoadBMP failed: " << SDL_GetError() << std::endl;

    Uint32 pink = SDL_MapSurfaceRGB(surface, 255, 0, 255);

    if (!SDL_SetSurfaceColorKey(surface, true, pink)) {
        std::cerr << "SDL_SetSurfaceColorKey failed: " << SDL_GetError() << std::endl;
        SDL_DestroySurface(surface);
        return;
    } 

    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    if (!m_texture)
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
   
    if (!SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND)) {
        std::cerr << "SDL_SetTextureBlendMode failed: " << SDL_GetError() << std::endl;
    }

}

Papyrus::Texture2D::Texture2D(SDL_Texture* texture)
    : m_texture{ texture }
{
    assert(m_texture != nullptr);
}
