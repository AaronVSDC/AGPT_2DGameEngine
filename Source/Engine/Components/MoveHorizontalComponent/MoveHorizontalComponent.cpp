#include "MoveHorizontalComponent.h"
#include "GameObject.h"

namespace Papyrus
{
    MoveHorizontalComponent::MoveHorizontalComponent(float speed, float reverseDistance)
        : m_speed(speed), m_reverseDistance(reverseDistance)
    {
    }

    void MoveHorizontalComponent::update(float deltaTime)
    {
        auto owner = getOwner();
        if (!owner) return;

        // Store starting X on first update
        if (m_startX == 0.f) m_startX = owner->m_Transform.position.x;

        // Move horizontally
        if (m_movingLeft)
            owner->m_Transform.position.x -= m_speed * deltaTime;
        else
            owner->m_Transform.position.x += m_speed * deltaTime;

        // Reverse logic based on distance
        float distanceMoved = std::abs(owner->m_Transform.position.x - m_startX);
        if (distanceMoved >= m_reverseDistance)
        {
            m_movingLeft = !m_movingLeft; // reverse direction
        }
    }
}
