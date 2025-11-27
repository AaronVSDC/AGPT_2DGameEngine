#include "BulletComponent.h"
#include "GameObject.h"

namespace Papyrus
{
    BulletComponent::BulletComponent(float speedPixelsPerSecond)
        : m_speed(speedPixelsPerSecond)
    {
    }

    void BulletComponent::fixedUpdate(float deltaTime) 
    {

        getOwner()->m_Transform.position.y -= m_speed * deltaTime;

        if (getOwner()->m_Transform.position.y < 100.0f)
        {
            getOwner()->markForRemoval(); 
        }
    }
}
