#include "EnemyBulletComponent.h"

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

    void EnemyBulletComponent::onTriggerEnter(GameObject* other)
    {
        std::cout << "EnemyBullet trigger with tag=" << (other ? other->getTag() : "null") << "\n";

        if (!other) return;

        if (other->getTag() != "Player")
            return;

        // kill bullet
        getOwner()->markForRemoval();

        // player explodes + dies
        explodeAndDie(
            other,
            "Resources/Textures/explode64.bmp",
            5, 2, 10, 16.0f
        );
    }
}
