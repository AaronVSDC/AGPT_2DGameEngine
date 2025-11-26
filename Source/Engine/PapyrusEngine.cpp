#include "PapyrusEngine.h"
#include "Window.h"

namespace Papyrus
{
	PapyrusEngine::PapyrusEngine()
	{
	}
	PapyrusEngine::~PapyrusEngine()
	{
	}

	void PapyrusEngine::run()
	{

		GWindow = std::make_unique<Window>(
			"Papyrus Engine",
			1280,
			720
		);

		SDL_Renderer* renderer = GWindow->getRenderer();

		bool running = true;
		SDL_Event e{};

		while (running)
		{
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_EVENT_QUIT)
					running = false;
			}

			SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
			SDL_RenderClear(renderer);

			SDL_RenderPresent(renderer);
		}

		GWindow.reset();
	}

	void PapyrusEngine::load()
	{
	}

}