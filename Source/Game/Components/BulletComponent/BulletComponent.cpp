#include "BulletComponent.h"
#include "GameObject.h"
#include "AnimationComponent.h"
#include "TextureComponent.h"
#include "ExplosionComponent.h"
#include <iostream> 
#include "BoxColliderComponent.h"
#include "AsteroidSplittingComponent.h"
#include "ExplosionUtility.h"

namespace xc
{
    BulletComponent::BulletComponent(float speedPixelsPerSecond)
        : m_speed(speedPixelsPerSecond)
    {
    }

    void BulletComponent::fixedUpdate(float deltaTime) 
    {

        getOwner()->m_Transform.position.y -= m_speed * deltaTime;

        if (getOwner()->m_Transform.position.y < -50.f)
        {
            getOwner()->markForRemoval(); 
        }
    }

    void BulletComponent::onTriggerEnter(Papyrus::GameObject* other)
    {

        if (!other) return;
        if (other->getTag() == "Indestructible")
        {
            getOwner()->markForRemoval();
            return;
        }
        else if (other->getTag() == "Aestroid")
        {
            if (other->getComponent<ExplosionComponent>())
            {
                getOwner()->markForRemoval();
                return;
            }

            //Asteroid Split
            auto* split = other->getComponent<AsteroidSplittingComponent>();
            if (split)
            {
                split->split();
            }

            getOwner()->markForRemoval();

            if (auto* enemyAnimation = other->getComponent<Papyrus::AnimationComponent>())
                enemyAnimation->m_Enabled = false;

            auto* enemyTexture = other->getComponent<Papyrus::TextureComponent>();
            if (!enemyTexture) return;

            enemyTexture->setTexture("Resources/Textures/explode64.bmp");

            auto explosionComponent = std::make_unique<ExplosionComponent>(5, 2, 10, 16.0f);
            ExplosionComponent* explosionRawPointer = explosionComponent.get();

            other->addComponent(std::move(explosionComponent));

            explosionRawPointer->start();
            explosionRawPointer->onEnable();
        }
        else if (other->getTag() == "Enemy")
        {
            if (other->getComponent<ExplosionComponent>())
            {
                getOwner()->markForRemoval();
                return;
            }

            getOwner()->markForRemoval();

            if (auto* enemyAnimation = other->getComponent<Papyrus::AnimationComponent>())
                enemyAnimation->m_Enabled = false;

            auto* enemyTexture = other->getComponent<Papyrus::TextureComponent>();
            if (!enemyTexture) return;

            enemyTexture->setTexture("Resources/Textures/explode64.bmp");

            auto explosionComponent = std::make_unique<ExplosionComponent>(5, 2, 10, 16.0f);
            ExplosionComponent* explosionRawPointer = explosionComponent.get();

            other->addComponent(std::move(explosionComponent));

            explosionRawPointer->start();
            explosionRawPointer->onEnable();
        }
    }

}