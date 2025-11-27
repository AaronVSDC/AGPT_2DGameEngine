#ifndef RENDERER_H 
#define RENDERER_H 

#include <SDL3/SDL.h>
#include "Singleton.h"

namespace Papyrus
{
    class Texture2D;

    class Renderer final : public Singleton<Renderer>
    {
        SDL_Renderer* m_Renderer{};
        SDL_Color     m_ClearColor{};
         
    public:
        void init();
        void render() const;
        void destroy();

        void renderTexture(const Texture2D& texture, float x, float y) const;
        void renderTexture(const Texture2D& texture, float x, float y, float rotationDegrees, SDL_FPoint* pivot = nullptr) const;
        void renderTexture(const Texture2D& texture, float x, float y, float width, float height, float rotationDegrees, SDL_FPoint* pivot = nullptr) const;

        SDL_Renderer* getSDLRenderer() const;

        const SDL_Color& getBackgroundColor() const { return m_ClearColor; }
        void setBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }
    };
}

#endif