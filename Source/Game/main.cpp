#include "main.h"
#include "PapyrusEngine.h"

#include "TextureComponent.h"
#include "KeyboardMoveCommand.h"
#include "AnimationComponent.h"
#include "PlayerAnimationComponent.h"
#include "PhysicsBodyComponent.h"
#include "BoxColliderComponent.h" 
#include "LonerShooterComponent.h"
#include "MoveVerticalComponent.h"
#include "MoveHorizontalComponent.h"
#include "EnemySpawnComponent.h"
#include "VerticalParallaxComponent.h"
#include "SceneManager.h"
#include "InputManager.h"




void load()
{
	auto mainLevel = Papyrus::SceneManager::getInstance().createScene("mainLevelScene");

	auto& input = Papyrus::InputManager::getInstance();

	auto background = std::make_unique< Papyrus::GameObject>();
	background->addComponent(std::make_unique<TextureComponent>("Resources/Textures/galaxy2.bmp"));
	background->m_Transform.position.x = 0.f;
	background->m_Transform.position.y = 0.f;
	mainLevel->add(std::move(background));

	auto parallax2 = std::make_unique< Papyrus::GameObject>();
	parallax2->addComponent(std::make_unique<TextureComponent>("Resources/Textures/galaxy4.bmp"));
	parallax2->addComponent(std::make_unique<VerticalParallaxComponent>(30.f));
	parallax2->m_Transform.position.x = 0.f;
	parallax2->m_Transform.position.y = -900.f;
	mainLevel->add(std::move(parallax2));

	auto parallax = std::make_unique< Papyrus::GameObject>();
	parallax->addComponent(std::make_unique<TextureComponent>("Resources/Textures/galaxy3.bmp"));
	parallax->addComponent(std::make_unique<VerticalParallaxComponent>(40.f));
	parallax->m_Transform.position.x = 0.f;
	parallax->m_Transform.position.y = -900.f;
	mainLevel->add(std::move(parallax));

	auto playerShip = std::make_unique< Papyrus::GameObject>();

	playerShip->addComponent(
		std::make_unique<TextureComponent>("Resources/Textures/Ship1.bmp")
	);

	playerShip->addComponent(
		std::make_unique<PlayerAnimationComponent>(
			7,
			12.0f
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

	playerShip->addComponent(std::make_unique<PhysicsBodyComponent>());
	playerShip->addComponent(std::make_unique<BoxColliderComponent>());

	playerShip->setTag("Player");

	auto enemySpawner = std::make_unique<GameObject>();
	enemySpawner->addComponent(std::make_unique<EnemySpawnerComponent>(3.f, GWindow->getWidth()));

	input.addController(0);


	input.addKeyboardCommand(SDL_SCANCODE_W, KeyState::Down, std::make_unique<MoveUpCommand>(playerShip.get()));
	input.addKeyboardCommand(SDL_SCANCODE_S, KeyState::Down, std::make_unique<MoveDownCommand>(playerShip.get()));
	input.addKeyboardCommand(SDL_SCANCODE_A, KeyState::Down, std::make_unique<MoveLeftCommand>(playerShip.get()));
	input.addKeyboardCommand(SDL_SCANCODE_D, KeyState::Down, std::make_unique<MoveRightCommand>(playerShip.get()));
	input.addKeyboardCommand(
		SDL_SCANCODE_SPACE,
		KeyState::Up,
		std::make_unique<ShootCommand>(playerShip.get())
	);
	input.addControllerCommand(0, ControllerButton::DPadUp, KeyState::Down, std::make_unique<MoveUpCommand>(playerShip.get()));
	input.addControllerCommand(0, ControllerButton::DPadDown, KeyState::Down, std::make_unique<MoveDownCommand>(playerShip.get()));
	input.addControllerCommand(0, ControllerButton::DPadLeft, KeyState::Down, std::make_unique<MoveLeftCommand>(playerShip.get()));
	input.addControllerCommand(0, ControllerButton::DPadRight, KeyState::Down, std::make_unique<MoveRightCommand>(playerShip.get()));
	input.addControllerCommand(0, ControllerButton::A, KeyState::Pressed, std::make_unique<ShootCommand>(playerShip.get()));
	input.addControllerStickCommand(0, Stick::Left, 0.2f, true, std::make_unique<AnalogMoveCommand>(playerShip.get()));
	mainLevel->add(std::move(playerShip));
	mainLevel->add(std::move(enemySpawner));
}



int main()
{
	Papyrus::PapyrusEngine engine{};

	try
	{
		engine.run();
	}
	catch (std::exception& e)
	{
		std::cout << "RUNTIME ERROR: " << e.what() << std::endl; 
	}
	return 0;
}
