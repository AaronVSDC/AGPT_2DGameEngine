#include <stdexcept>
#include <cstring>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "Window.h"
#include <iostream>


namespace Papyrus
{
    Renderer::Renderer()
    {
        init(); 
    }
    void Renderer::init()
    {
        m_Renderer = SDL_CreateRenderer(
            GWindow->getHandle(),
            NULL
        );

        if (!m_Renderer)
            std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    }

    void Renderer::render() const
    {
        const auto& color = getBackgroundColor();



        SDL_SetRenderDrawColor(
            m_Renderer,
            color.r,
            color.g,
            color.b,
            color.a
        );

        SDL_RenderClear(m_Renderer);

        SceneManager::getInstance().render();

        SDL_RenderPresent(m_Renderer);
    }

    void Renderer::destroy()
    {
        if (m_Renderer)
        {
            SDL_DestroyRenderer(m_Renderer);
            m_Renderer = nullptr;
        }
    }

    // ------------------------
    // TEXTURE RENDERING
    // ------------------------

    void Renderer::renderTexture(const Texture2D& texture, float x, float y) const
    {
        float w = 0.0f;
        float h = 0.0f;

        SDL_GetTextureSize(texture.getSDLTexture(), &w, &h);

        SDL_FRect dst{ x, y, w, h };

        SDL_RenderTexture(
            m_Renderer,
            texture.getSDLTexture(),
            nullptr,
            &dst
        );
    }

    void  Renderer::renderTexture(
        const Texture2D& texture,
        float x,
        float y,
        float rotationDegrees,
        SDL_FPoint* pivot
    ) const
    {
        float w = 0.0f;
        float h = 0.0f;

        SDL_GetTextureSize(texture.getSDLTexture(), &w, &h);

        SDL_FRect dst{ x, y, w, h };

        SDL_RenderTextureRotated(
            m_Renderer,
            texture.getSDLTexture(),
            nullptr,
            &dst,
            rotationDegrees,
            pivot,
            SDL_FLIP_NONE
        );
    }

    void Renderer::renderTexture(
        const Texture2D& texture,
        float x,
        float y,
        float width,
        float height,
        float rotationDegrees,
        SDL_FPoint* pivot
    ) const
    {
        SDL_FRect dst{ x, y, width, height };

        SDL_RenderTextureRotated(
            m_Renderer,
            texture.getSDLTexture(),
            nullptr,
            &dst,
            rotationDegrees,
            pivot,
            SDL_FLIP_NONE
        );
    }

    void Renderer::renderTexture(
        const Texture2D& texture,
        const SDL_FRect& sourceRect,
        float x,
        float y,
        float width,
        float height,
        float rotationDegrees,
        SDL_FPoint* pivot
    ) const
    {
        SDL_FRect destinationRect{ x, y, width, height };

        SDL_RenderTextureRotated(
            m_Renderer,
            texture.getSDLTexture(),
            &sourceRect,
            &destinationRect,
            rotationDegrees,
            pivot,
            SDL_FLIP_NONE
        );
    }

    SDL_Renderer* Renderer::getSDLRenderer() const
    {
        return m_Renderer;
    }
}