#include "Window.h"
#include <stdexcept>

namespace Papyrus
{
	std::unique_ptr<Window> GWindow = nullptr;

	Window::Window(const std::string& title, int width, int height)
		: m_WIDTH(width), m_HEIGHT(height)
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
			throw std::runtime_error(SDL_GetError());

		m_SDLWindow = SDL_CreateWindow(
			title.c_str(),
			m_WIDTH,
			m_HEIGHT,
			SDL_WINDOW_RESIZABLE
		);

		if (!m_SDLWindow)
			throw std::runtime_error(SDL_GetError());

		m_Renderer = SDL_CreateRenderer(
			m_SDLWindow,
			NULL		
		);

		if (!m_Renderer)
			throw std::runtime_error(SDL_GetError());
	}

	Window::~Window()
	{
		if (m_Renderer)
			SDL_DestroyRenderer(m_Renderer);

		if (m_SDLWindow)
			SDL_DestroyWindow(m_SDLWindow);

		SDL_Quit();
	}
}