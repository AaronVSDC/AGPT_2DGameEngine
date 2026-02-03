#include "EnemyBulletComponent.h"
#include "HealthUIComponent.h"
#include "HealthComponent.h"

#include "GameObject.h"
#include "ExplosionUtility.h"
#include <iostream>

namespace xc
{
    EnemyBulletComponent::EnemyBulletComponent(float speedPixelsPerSecond)
        : m_speed(speedPixelsPerSecond)
    {
    }

    void EnemyBulletComponent::fixedUpdate(float fixedDeltaTime)
    {
        getOwner()->m_Transform.position.y += m_speed * fixedDeltaTime;

        if (getOwner()->m_Transform.position.y > 800.0f) // offscreen bottom (adjust if needed)
        {
            getOwner()->markForRemoval();
        }


    }

    void EnemyBulletComponent::onTriggerEnter(Papyrus::GameObject* other)
    {
        std::cout << "EnemyBullet trigger with tag=" << (other ? other->getTag() : "null") << "\n";

        if (!other) return;

        if (other->getTag() == "Player")
        {

            // kill bullet
            getOwner()->markForRemoval(); 

            auto* health = other->getComponent<HealthComponent>();
            if (health)
            {
                health->damage(1); // remove 1 HP

                // Remove bullet
                getOwner()->markForRemoval();

                // If player is dead, explode
                if (health->isDead())
                {
                    explodeAndDie(
                        other,
                        "Resources/Textures/explode64.bmp",
                        5, 2, 10, 16.0f
                    );
                }
            }
        }
        else if (other->getTag() == "Companion")
        {
            getOwner()->markForRemoval(); 

            explodeAndDie( 
                other, 
                "Resources/Textures/explode64.bmp",
                5, 2, 10, 16.0f
            );
        }
    }
}
