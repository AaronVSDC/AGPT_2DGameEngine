#include "Window.h"
#include <stdexcept>

#include <glad/glad.h>

namespace Papyrus
{ 
    std::unique_ptr<Window> GWindow = nullptr;

    Window::Window(const std::string& title, int width, int height)
        : m_Width(width), m_Height(height)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        // (Optional) MSAA
        // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

        m_Window = SDL_CreateWindow(
            title.c_str(),
            width,
            height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
        );
        if (!m_Window) throw std::runtime_error(SDL_GetError());

        m_GLContext = SDL_GL_CreateContext(m_Window);
        if (!m_GLContext) throw std::runtime_error(SDL_GetError());

        SDL_GL_MakeCurrent(m_Window, m_GLContext);

        SDL_GL_SetSwapInterval(1);

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
            throw std::runtime_error("gladLoadGLLoader failed");
    }

    Window::~Window()
    {
        if (m_GLContext) SDL_GL_DestroyContext(m_GLContext);
        if (m_Window) SDL_DestroyWindow(m_Window);
        m_GLContext = nullptr;
        m_Window = nullptr;
    }
}
