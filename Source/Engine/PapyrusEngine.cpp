#include "PapyrusEngine.h"
#include "Renderer.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Window.h"
#include "PhysicsManager.h" 
#include <thread>
#include "TextureComponent.h"

namespace Papyrus
{
	PapyrusEngine::PapyrusEngine()
	{
		GWindow = std::make_unique<Window>(
			"Papyrus Engine",
			1280,
			720
		);
	}
	PapyrusEngine::~PapyrusEngine()
	{
		GWindow.reset();
	}

	void PapyrusEngine::run()
	{

		load();

		constexpr int desiredFPS{ 60 };
		constexpr int frameTimeMs{ 1000 / desiredFPS };

		auto& renderer = Renderer::getInstance();
		auto& sceneManager = SceneManager::getInstance();
		auto& input = InputManager::getInstance();
		auto& physicsManager = PhysicsManager::getInstance();

		const float fixedTimeStep{ 0.02f };

		bool doContinue = true;
		auto lastTime = std::chrono::high_resolution_clock::now();
		float lag = 0.0f;

		sceneManager.onEnable();
		sceneManager.start();
		while (doContinue)
		{
			const auto currentTime = std::chrono::high_resolution_clock::now();
			const float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
			lastTime = currentTime;
			lag += deltaTime;

			doContinue = input.processInput();

			while (lag >= fixedTimeStep)
			{
				sceneManager.fixedUpdate(fixedTimeStep);
				physicsManager.fixedUpdate(fixedTimeStep);
				lag -= fixedTimeStep;
			}
			sceneManager.update(deltaTime);
			renderer.render();

			const auto sleepTime = currentTime + std::chrono::milliseconds(frameTimeMs) - std::chrono::high_resolution_clock::now();
			std::this_thread::sleep_for(sleepTime);
		}
		sceneManager.onDisable();


	}

	void PapyrusEngine::load()
	{
		auto mainLevel = SceneManager::getInstance().createScene("mainLevelScene");

		auto ship = std::make_unique<GameObject>();

		ship->addComponent(std::make_unique<TextureComponent>("Resources/Textures/Ship1.bmp")); 
		

		mainLevel->add(std::move(ship)); 


	}

}