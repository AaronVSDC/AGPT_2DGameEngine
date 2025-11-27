#ifndef MOVE_UP_COMMAND_H
#define MOVE_UP_COMMAND_H

#include "Command.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "BulletComponent.h"
#include "AnimationComponent.h"
#include "PhysicsBodyComponent.h"
#include "BoxColliderComponent.h"

namespace Papyrus
{
    class MoveUpCommand final : public Command 
    {
    public:
        explicit MoveUpCommand(GameObject* gameObject)  
            : m_gameObject(gameObject) {} 

        void execute() override 
        { 
            if (!m_gameObject) return;

            auto* moveComponent = m_gameObject->getComponent<MoveComponent>();
            if (!moveComponent) return;

            moveComponent->addAcceleration({ 0.0f, -1.0f });
        }

    private:
        GameObject* m_gameObject{};
    };


    class MoveDownCommand final : public Command
    {
    public:
        explicit MoveDownCommand(GameObject* gameObject)
            : m_gameObject(gameObject) {}

        void execute() override
        {
            if (!m_gameObject) return;

            auto* moveComponent = m_gameObject->getComponent<MoveComponent>();
            if (!moveComponent) return;

            moveComponent->addAcceleration({ 0.0f, 1.0f });
        }

    private:
        GameObject* m_gameObject{};
    };

    class MoveLeftCommand final : public Command 
    {
    public:
        explicit MoveLeftCommand(GameObject* gameObject)
            : m_gameObject(gameObject) {}

        void execute() override
        {
            if (!m_gameObject) return;

            auto* moveComponent = m_gameObject->getComponent<MoveComponent>();
            if (!moveComponent) return;

            moveComponent->addAcceleration({ -1.0f, 0.0f });
        }

    private:
        GameObject* m_gameObject{};
    };

    class MoveRightCommand final : public Command
    { 
    public:
        explicit MoveRightCommand(GameObject* gameObject)
            : m_gameObject(gameObject) {}

        void execute() override
        {
            if (!m_gameObject) return;

            auto* moveComponent = m_gameObject->getComponent<MoveComponent>();
            if (!moveComponent) return;

            moveComponent->addAcceleration({ 1.0f, 0.0f });
        }

    private:
        GameObject* m_gameObject{};
    };


    class ShootCommand final : public Command
    {
    public:
        explicit ShootCommand(GameObject* player)
            : m_player(player) {}

        void execute() override
        {
            if (!m_player)
                return;

            auto bullet = std::make_unique<GameObject>();

            const b2Vec2 playerPos = m_player->m_Transform.position;

            auto* playerTexture = m_player->getComponent<TextureComponent>();
            if (!playerTexture)
                return;

            const b2Vec2 playerSize = playerTexture->getSize();

            bullet->m_Transform.position = {
                playerPos.x + 15,
                playerPos.y
            };

            bullet->addComponent(
                std::make_unique<TextureComponent>("Resources/Textures/missile.bmp")
            );

            bullet->addComponent(
                std::make_unique<BulletComponent>(800.0f) // speed (px/s)
            );

            bullet->addComponent(std::make_unique<PhysicsBodyComponent>()); 
            bullet->addComponent(std::make_unique<BoxColliderComponent>()); 

            bullet->addComponent(
                std::make_unique<AnimationComponent>(2, 1, 2, 2));
            bullet->start();
            bullet->onEnable();

            SceneManager::getInstance()
                .getCurrentScene()  
                ->add(std::move(bullet));


        }

    private:
        GameObject* m_player{};
    };
}

#endif
