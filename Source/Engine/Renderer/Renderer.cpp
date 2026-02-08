#include "Renderer.h"
#include "Texture2D.h"
#include "SceneManager.h"
#include "Window.h"

#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include <iostream>

namespace Papyrus
{
    static const char* kVS = R"GLSL(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aUV;

    uniform mat4 uMVP;

    out vec2 vUV;

    void main()
    {
        vUV = aUV;
        gl_Position = uMVP * vec4(aPos, 0.0, 1.0);
    }
    )GLSL";

    static const char* kFS = R"GLSL(
#version 330 core
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uTex;
uniform vec2 uUVOffset;
uniform vec2 uUVScale;

void main()
{
    vec2 uv = vUV * uUVScale + uUVOffset;
    vec4 tex = texture(uTex, uv);

    // exact magenta key
    if (tex.rgb == vec3(1.0, 0.0, 1.0))
        discard;

    FragColor = tex;
}
    )GLSL";

    Renderer::Renderer()
    {
        init();
    }

    static GLuint CompileShader(GLenum type, const char* src)
    {
        GLuint s = glCreateShader(type);
        glShaderSource(s, 1, &src, nullptr);
        glCompileShader(s);

        GLint ok = 0;
        glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
        if (!ok)
        {
            char log[2048];
            glGetShaderInfoLog(s, 2048, nullptr, log);
            std::cerr << "Shader compile error:\n" << log << "\n";
            glDeleteShader(s);
            throw std::runtime_error("Shader compilation failed");
        }
        return s;
    }

    GLuint Renderer::buildProgram(const char* vsSrc, const char* fsSrc) const
    {
        GLuint vs = CompileShader(GL_VERTEX_SHADER, vsSrc);
        GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fsSrc);

        GLuint p = glCreateProgram();
        glAttachShader(p, vs);
        glAttachShader(p, fs);
        glLinkProgram(p);

        glDeleteShader(vs);
        glDeleteShader(fs);

        GLint ok = 0;
        glGetProgramiv(p, GL_LINK_STATUS, &ok);
        if (!ok)
        {
            char log[2048];
            glGetProgramInfoLog(p, 2048, nullptr, log);
            std::cerr << "Program link error:\n" << log << "\n";
            glDeleteProgram(p);
            throw std::runtime_error("Program linking failed");
        }
        return p;
    }

    glm::mat4 Renderer::projection() const
    {
        // coordinates: (0,0) top-left, +x right, +y down
        auto& window = Window::getInstance();
        const float w = (float)window.getWidth();
        const float h = (float)window.getHeight();
        glm::mat4 P = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);
         
        constexpr float kSceneRotationDegrees = 90.0f;
        const glm::vec2 center{ w * 0.5f, h * 0.5f };

        glm::mat4 V(1.0f);
        V = glm::translate(V, glm::vec3(center, 0.0f));
        V = glm::rotate(V, glm::radians(kSceneRotationDegrees), glm::vec3(0, 0, 1));
        V = glm::translate(V, glm::vec3(-center, 0.0f));

        return P * V;
    }

    void Renderer::init()
    {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_Program = buildProgram(kVS, kFS);
        glUseProgram(m_Program);

        m_uMVP = glGetUniformLocation(m_Program, "uMVP");
        m_uTex = glGetUniformLocation(m_Program, "uTex");
        m_uUVOffset = glGetUniformLocation(m_Program, "uUVOffset");
        m_uUVScale = glGetUniformLocation(m_Program, "uUVScale");

        glUniform1i(m_uTex, 0);


        const float verts[] = {
            // pos      // uv
            0.f, 0.f,   0.f, 0.f,
            1.f, 0.f,   1.f, 0.f,
            1.f, 1.f,   1.f, 1.f,

            0.f, 0.f,   0.f, 0.f,
            1.f, 1.f,   1.f, 1.f,
            0.f, 1.f,   0.f, 1.f
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Renderer::render() const
    {
        glViewport(0, 0, Window::getInstance().getWidth(), Window::getInstance().getHeight());

        const auto& c = m_ClearColor;
        glClearColor(c.r / 255.f, c.g / 255.f, c.b / 255.f, c.a / 255.f);
        glClear(GL_COLOR_BUFFER_BIT);

        SceneManager::getInstance().render();

        SDL_GL_SwapWindow(Window::getInstance().getHandle()); 
    }

    void Renderer::destroy()
    {
        if (m_VBO) glDeleteBuffers(1, &m_VBO);
        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
        if (m_Program) glDeleteProgram(m_Program);

        m_VBO = m_VAO = m_Program = 0;
    }

    void Renderer::drawInternal(
        const Texture2D& tex,
        const SDL_FRect& srcPixels,
        const SDL_FRect& dstPixels,
        float rotationDegrees,
        const SDL_FPoint* pivot
    ) const
    {
        glm::mat4 P = projection();
        glm::mat4 M(1.0f);

        M = glm::translate(M, glm::vec3(dstPixels.x, dstPixels.y, 0.0f));

        glm::vec2 piv = pivot
            ? glm::vec2(pivot->x, pivot->y)
            : glm::vec2(dstPixels.w * 0.5f, dstPixels.h * 0.5f);

        M = glm::translate(M, glm::vec3(piv, 0.0f));
        M = glm::rotate(M, glm::radians(rotationDegrees), glm::vec3(0, 0, 1));
        M = glm::translate(M, glm::vec3(-piv, 0.0f));

        M = glm::scale(M, glm::vec3(dstPixels.w, dstPixels.h, 1.0f));

        glm::mat4 MVP = P * M;

        const float Tw = (float)tex.width();
        const float Th = (float)tex.height();

        const float sx = srcPixels.x;
        const float sy = srcPixels.y;
        const float sw = srcPixels.w;
        const float sh = srcPixels.h;

        glm::vec2 uvOffset(sx / Tw, sy / Th);   // no flip
        glm::vec2 uvScale(sw / Tw, sh / Th); 

        glUseProgram(m_Program);
        glUniformMatrix4fv(m_uMVP, 1, GL_FALSE, &MVP[0][0]);
        glUniform2fv(m_uUVScale, 1, &uvScale[0]);
        glUniform2fv(m_uUVOffset, 1, &uvOffset[0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex.getGLTexture());

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6); 
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Renderer::renderTexture(const Texture2D& texture, float x, float y) const
    {
        auto sz = texture.getSize();
        SDL_FRect src{ 0.f, 0.f, sz.x, sz.y };
        SDL_FRect dst{ x, y, sz.x, sz.y };
        drawInternal(texture, src, dst, 0.0f, nullptr);
    }

    void Renderer::renderTexture(const Texture2D& texture, float x, float y, float rotationDegrees, SDL_FPoint* pivot) const
    {
        auto sz = texture.getSize();
        SDL_FRect src{ 0.f, 0.f, sz.x, sz.y };
        SDL_FRect dst{ x, y, sz.x, sz.y };
        drawInternal(texture, src, dst, rotationDegrees, pivot);
    }

    void Renderer::renderTexture(const Texture2D& texture, float x, float y, float width, float height, float rotationDegrees, SDL_FPoint* pivot) const
    {
        SDL_FRect src{ 0.f, 0.f, (float)texture.width(), (float)texture.height() };
        SDL_FRect dst{ x, y, width, height };
        drawInternal(texture, src, dst, rotationDegrees, pivot); 
    }

    void Renderer::renderTexture(const Texture2D& texture, const SDL_FRect& sourceRect, float x, float y, float width, float height, float rotationDegrees, SDL_FPoint* pivot) const
    {
        SDL_FRect dst{ x, y, width, height };
        drawInternal(texture, sourceRect, dst, rotationDegrees, pivot);
    }
}
