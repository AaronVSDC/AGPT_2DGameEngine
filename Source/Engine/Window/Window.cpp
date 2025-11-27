#include "Window.h"
#include <iostream>
namespace Papyrus
{
	std::unique_ptr<Window> GWindow = nullptr;

	Window::Window(const std::string& title, int width, int height)
		: m_WIDTH(width), m_HEIGHT(height)
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
			std::cerr << SDL_GetError() << std::endl;

		m_SDLWindow = SDL_CreateWindow(
			title.c_str(),
			m_WIDTH,
			m_HEIGHT,
			SDL_WINDOW_RESIZABLE
		);

		if (!m_SDLWindow)
			throw std::runtime_error(SDL_GetError());


	}

	Window::~Window()
	{
		if (m_SDLWindow)
			SDL_DestroyWindow(m_SDLWindow);
	}
}