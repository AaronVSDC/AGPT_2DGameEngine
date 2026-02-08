#ifndef MOVE_UP_COMMAND_H
#define MOVE_UP_COMMAND_H

#include <cmath>
#include <memory>

#include "Command.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "BulletComponent.h"
#include "AnimationComponent.h"
#include "PhysicsBodyComponent.h"
#include "BoxColliderComponent.h"
#include "SceneManager.h"
#include "TextureComponent.h"

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

            moveComponent->addAcceleration({ -1.0f, 0.0f }); 
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

            moveComponent->addAcceleration({ 1.0f, 0.0f });
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

            moveComponent->addAcceleration({ 0.0f, 1.0f });
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

            moveComponent->addAcceleration({ 0.0f, -1.0f });
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

            auto playerBullet = std::make_unique<GameObject>();

            const b2Vec2 playerPos = m_player->m_Transform.position;

            auto* playerTexture = m_player->getComponent<TextureComponent>();
            if (!playerTexture)
                return;

            // Previously spawned "forward" along +X. For +90° world mapping, forward is along -X.
            playerBullet->m_Transform.position = {
                playerPos.x - 15.0f,
                playerPos.y
            };

            playerBullet->addComponent(
                std::make_unique<TextureComponent>("Resources/Textures/missile.bmp")
            );
            playerBullet->addComponent(
                std::make_unique<AnimationComponent>(2, 1, 2, 2));

            playerBullet->addComponent(std::make_unique<PhysicsBodyComponent>());
            playerBullet->addComponent(std::make_unique<BoxColliderComponent>());

            playerBullet->addComponent(
                std::make_unique<xc::BulletComponent>(800.0f) // speed (px/s)
            );

            playerBullet->start();
            playerBullet->onEnable();
            SceneManager::getInstance()
                .getCurrentScene()
                ->add(std::move(playerBullet));

            // companion bullets
            auto companions = SceneManager::getInstance().getCurrentScene()->findGameObjectsByTag("Companion");
            if (companions.empty()) return;

            for (auto& companion : companions)
            {
                auto companionBullet = std::make_unique<GameObject>();

                const b2Vec2 companionPos = companion->m_Transform.position;

                auto* companionTexture = companion->getComponent<TextureComponent>();
                if (!companionTexture)
                    return;

                companionBullet->m_Transform.position = {
                    companionPos.x - 15.0f,
                    companionPos.y
                };

                companionBullet->addComponent(
                    std::make_unique<TextureComponent>("Resources/Textures/missile.bmp")
                );
                companionBullet->addComponent(
                    std::make_unique<AnimationComponent>(2, 1, 2, 2));

                companionBullet->addComponent(std::make_unique<PhysicsBodyComponent>());
                companionBullet->addComponent(std::make_unique<BoxColliderComponent>());

                companionBullet->addComponent(
                    std::make_unique<xc::BulletComponent>(800.0f) // speed (px/s)
                );

                companionBullet->start();
                companionBullet->onEnable();
                SceneManager::getInstance()
                    .getCurrentScene()
                    ->add(std::move(companionBullet));
            }
        }

    private:
        GameObject* m_player{};
    };


    class AnalogMoveCommand final : public AnalogCommand
    {
    public:
        explicit AnalogMoveCommand(GameObject* gameObject)
            : m_gameObject(gameObject)
        {
        }

        void execute(const b2Vec2& value) override
        {
            if (!m_gameObject)
                return;

            auto* moveComponent = m_gameObject->getComponent<MoveComponent>();
            if (!moveComponent)
                return;

            const float magnitude = std::sqrt(value.x * value.x + value.y * value.y);
            if (magnitude <= 0.0f)
                return;

            // Keep controller Y flipped (common for screen-space)
            const b2Vec2 normalizedDirection{ value.x / magnitude, -value.y / magnitude };

            // Was rotating +90° (for -90° world). For +90° world, rotate -90° instead:
            // (x, y) rotated -90° => (y, -x)
            const b2Vec2 rotatedDirection{ normalizedDirection.y, -normalizedDirection.x };

            moveComponent->addAcceleration(rotatedDirection);
        }

    private:
        GameObject* m_gameObject{};
    };
}

#endif
