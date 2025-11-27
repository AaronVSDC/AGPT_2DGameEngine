#include "PapyrusEngine.h"
#include "Renderer.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Window.h"
#include "PhysicsManager.h" 
#include <thread>
#include "TextureComponent.h"
#include "KeyboardMoveCommand.h"
#include "AnimationComponent.h"
#include "PlayerAnimationComponent.h"

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

		auto& input = Papyrus::InputManager::getInstance(); 
		auto playerShip = std::make_unique<GameObject>();

		playerShip->addComponent(
			std::make_unique<TextureComponent>("Resources/Textures/Ship1.bmp")
		);

		// 7 frames laid out in 7 columns × 1 row
		playerShip->addComponent(
			std::make_unique<PlayerAnimationComponent>( 
				7,      // columns (L2, L1, IDLE, R1, R2)
				12.0f   // FPS 
			)
		);

		// Movement
		playerShip->addComponent(
			std::make_unique<MoveComponent>(
				400.0f,
				1600.0f,
				2000.0f
			)
		);

		auto loner = std::make_unique<GameObject>(); 

		loner->addComponent(std::make_unique<TextureComponent>("Resources/Textures/LonerA.bmp")); 
		loner->addComponent(std::make_unique<AnimationComponent>(4, 4, 16, 8)); 

		auto rusher = std::make_unique<GameObject>();

		rusher->addComponent(std::make_unique<TextureComponent>("Resources/Textures/rusher.bmp"));
		rusher->addComponent(std::make_unique<AnimationComponent>(4, 6, 4*6, 8));

		rusher->m_Transform.position.x = 120.f; 



		input.addKeyboardCommand(SDL_SCANCODE_W, KeyState::Down, std::make_unique<MoveUpCommand>(playerShip.get()));
		input.addKeyboardCommand(SDL_SCANCODE_S, KeyState::Down, std::make_unique<MoveDownCommand>(playerShip.get()));
		input.addKeyboardCommand(SDL_SCANCODE_A, KeyState::Down, std::make_unique<MoveLeftCommand>(playerShip.get()));
		input.addKeyboardCommand(SDL_SCANCODE_D, KeyState::Down, std::make_unique<MoveRightCommand>(playerShip.get()));
		input.addKeyboardCommand(
			SDL_SCANCODE_SPACE, 
			KeyState::Up,
			std::make_unique<ShootCommand>(playerShip.get()) 
		);
		mainLevel->add(std::move(playerShip));
		mainLevel->add(std::move(rusher)); 
		mainLevel->add(std::move(loner)); 
	}

}