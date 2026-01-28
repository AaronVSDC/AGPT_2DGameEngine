#include "main.h"
#include <DecideCollisionComponent.h>

void load()
{
	auto mainLevel = Papyrus::SceneManager::getInstance().createScene("mainLevelScene");   

	auto& input = Papyrus::InputManager::getInstance();
	auto& window = Papyrus::Window::getInstance(); 
	 

	//---------------------------------------------------------------------------------------------------------
	//BACKGROUND AND PARRALLAX EFFECT
	//---------------------------------------------------------------------------------------------------------
	auto background = std::make_unique< Papyrus::GameObject>();
	background->addComponent(std::make_unique<Papyrus::TextureComponent>("Resources/Textures/galaxy2.bmp"));
	background->m_Transform.position.x = 0.f; 
	background->m_Transform.position.y = 0.f;  
	mainLevel->add(std::move(background)); 

	auto parallax2 = std::make_unique< Papyrus::GameObject>(); 
	parallax2->addComponent(std::make_unique<Papyrus::TextureComponent>("Resources/Textures/galaxy4.bmp")); 
	parallax2->addComponent(std::make_unique<xc::VerticalParallaxComponent>(30.f));    
	parallax2->m_Transform.position.x = 0.f;
	parallax2->m_Transform.position.y = -900.f;
	mainLevel->add(std::move(parallax2));

	auto parallax = std::make_unique< Papyrus::GameObject>();
	parallax->addComponent(std::make_unique<Papyrus::TextureComponent>("Resources/Textures/galaxy3.bmp")); 
	parallax->addComponent(std::make_unique<xc::VerticalParallaxComponent>(40.f));  
	parallax->m_Transform.position.x = 0.f;  
	parallax->m_Transform.position.y = -900.f; 
	mainLevel->add(std::move(parallax)); 

	//-----------------------------------------------------------------------------------------------------------
	//PLAYER
	//-----------------------------------------------------------------------------------------------------------
	auto playerShip = std::make_unique< Papyrus::GameObject>(); 
	playerShip->addComponent( std::make_unique<Papyrus::TextureComponent>("Resources/Textures/Ship1.bmp ")); 
	playerShip->addComponent( std::make_unique<Papyrus::PlayerAnimationComponent>( 7,12.0f));
	playerShip->addComponent(std::make_unique<Papyrus::MoveComponent>(400.0f,1600.0f,2000.0f));
	playerShip->addComponent(std::make_unique<Papyrus::PhysicsBodyComponent>());
	playerShip->addComponent(std::make_unique<Papyrus::BoxColliderComponent>());
	playerShip->addComponent(std::make_unique<xc::DecideCollisionComponent>()); 
	playerShip->setTag("Player");


	//----------------------------------------------------------------------------------------------------------
	//COMPANION
	//----------------------------------------------------------------------------------------------------------
	auto companion = std::make_unique<Papyrus::GameObject>(); 
	companion->addComponent(std::make_unique<Papyrus::TextureComponent>("Resources/Textures/clone.bmp"));  
	companion->addComponent(std::make_unique<Papyrus::AnimationComponent>(4, 4, 4*4, 14.f));  
	companion->addComponent(std::make_unique<xc::CompanionBehaviorComponent>(glm::vec2{ 55,55 })); 
	companion->addComponent(std::make_unique<Papyrus::PhysicsBodyComponent>()); 
	companion->addComponent(std::make_unique<Papyrus::BoxColliderComponent>()); 
	companion->addComponent(std::make_unique<xc::DecideCollisionComponent>()); 
	companion->setTag("Companion"); 

	//------------------------------------------------------------------------------------------------
	//ENEMY SPAWNER
	//------------------------------------------------------------------------------------------------
	auto enemySpawner = std::make_unique<Papyrus::GameObject>();
	enemySpawner->addComponent(std::make_unique<xc::PowerUpSpawnComponent>(3.f, window.getWidth()));

	//------------------------------------------------------------------------------------------------
	//POWERUP SPAWNER
	//------------------------------------------------------------------------------------------------
	auto powerUpSpawner = std::make_unique<Papyrus::GameObject>();
	powerUpSpawner->addComponent(std::make_unique<xc::EnemySpawnerComponent>(3.f, window.getWidth()));



	//-------------------------------------------------------------------------------------------------------
	//INPUT ASSIGNMENT/ CONTROLLER BINDING
	//-------------------------------------------------------------------------------------------------------

	input.addController(0);
	input.addKeyboardCommand(SDL_SCANCODE_W, Papyrus::KeyState::Down, std::make_unique<Papyrus::MoveUpCommand>(playerShip.get()));
	input.addKeyboardCommand(SDL_SCANCODE_S, Papyrus::KeyState::Down, std::make_unique<Papyrus::MoveDownCommand>(playerShip.get()));
	input.addKeyboardCommand(SDL_SCANCODE_A, Papyrus::KeyState::Down, std::make_unique<Papyrus::MoveLeftCommand>(playerShip.get()));
	input.addKeyboardCommand(SDL_SCANCODE_D, Papyrus::KeyState::Down, std::make_unique<Papyrus::MoveRightCommand>(playerShip.get()));
	input.addKeyboardCommand(SDL_SCANCODE_SPACE,Papyrus::KeyState::Up,std::make_unique<Papyrus::ShootCommand>(playerShip.get()));

	input.addControllerCommand(0, Papyrus::ControllerButton::DPadUp, Papyrus::KeyState::Down, std::make_unique<Papyrus::MoveUpCommand>(playerShip.get()));
	input.addControllerCommand(0, Papyrus::ControllerButton::DPadDown, Papyrus::KeyState::Down, std::make_unique<Papyrus::MoveDownCommand>(playerShip.get()));
	input.addControllerCommand(0, Papyrus::ControllerButton::DPadLeft, Papyrus::KeyState::Down, std::make_unique<Papyrus::MoveLeftCommand>(playerShip.get()));
	input.addControllerCommand(0, Papyrus::ControllerButton::DPadRight, Papyrus::KeyState::Down, std::make_unique<Papyrus::MoveRightCommand>(playerShip.get()));
	input.addControllerCommand(0, Papyrus::ControllerButton::A, Papyrus::KeyState::Pressed, std::make_unique<Papyrus::ShootCommand>(playerShip.get()));
	input.addControllerStickCommand(0, Papyrus::Stick::Left, 0.2f, true, std::make_unique<Papyrus::AnalogMoveCommand>(playerShip.get()));
	
	

	//----------------------------------------
	//ADD EVERYTHING TO THE SCENE
	//----------------------------------------
	mainLevel->add(std::move(playerShip));
	mainLevel->add(std::move(enemySpawner));
	mainLevel->add(std::move(powerUpSpawner));
	mainLevel->add(std::move(companion)); 
}



int main()
{
	Papyrus::PapyrusEngine engine{};

	try
	{
		engine.run(load); 
	}
	catch (std::exception& e)
	{
		std::cout << "RUNTIME ERROR: " << e.what() << std::endl; 
	}
	return 0;
}
