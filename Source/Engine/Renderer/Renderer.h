#ifndef RENDERER_H 
#define RENDERER_H 
#include <SDL3/SDL.h>
#include "Singleton.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Papyrus
{
    class Texture2D;

    class Renderer final : public Singleton<Renderer>
    {
    public:
        Renderer();
        void init();
        void render() const;
        void destroy();

        void renderTexture(const Texture2D& texture, float x, float y) const;
        void renderTexture(const Texture2D& texture, float x, float y, float rotationDegrees, SDL_FPoint* pivot = nullptr) const;
        void renderTexture(const Texture2D& texture, float x, float y, float width, float height, float rotationDegrees, SDL_FPoint* pivot = nullptr) const;
        void renderTexture(const Texture2D& texture, const SDL_FRect& sourceRect, float x, float y, float width, float height, float rotationDegrees = 0.0f, SDL_FPoint* pivot = nullptr) const;

        const SDL_Color& getBackgroundColor() const { return m_ClearColor; }
        void setBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }

    private:
        SDL_Color m_ClearColor{ 0,0,0,255 };

        GLuint m_Program = 0;
        GLuint m_VAO = 0;
        GLuint m_VBO = 0;

        GLint m_uMVP = -1;
        GLint m_uTex = -1;
        GLint m_uUVOffset = -1;
        GLint m_uUVScale = -1;

        glm::mat4 projection() const;

        GLuint buildProgram(const char* vsSrc, const char* fsSrc) const;

        void drawInternal(
            const Texture2D& tex,
            const SDL_FRect& srcPixels,
            const SDL_FRect& dstPixels,
            float rotationDegrees,
            const SDL_FPoint* pivot
        ) const;
    };
}

#endif