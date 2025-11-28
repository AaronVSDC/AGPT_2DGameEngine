#include "MoveVerticalComponent.h"
#include "GameObject.h"

namespace Papyrus
{
    MoveVerticalComponent::MoveVerticalComponent(float speed, float reverseDistance)
        : m_speed(speed), m_reverseDistance(reverseDistance)
    {
    }

    void MoveVerticalComponent::update(float deltaTime)
    {
        auto owner = getOwner();
        if (!owner) return;

        // On first frame, store starting Y
        if (m_startY == 0.f) m_startY = owner->m_Transform.position.y;

        // Move
        if (m_movingDown)
            owner->m_Transform.position.y += m_speed * deltaTime;
        else
            owner->m_Transform.position.y -= m_speed * deltaTime;

        // Reverse logic based on distance
        if (owner->getTag() == "Enemy")
        {
            float distanceMoved = owner->m_Transform.position.y - m_startY;
            if (m_movingDown && distanceMoved >= m_reverseDistance)
            {
                m_movingDown = false; // go up
            }
            else if (!m_movingDown && distanceMoved <= 0.f)
            {
                m_movingDown = true; // go down again (optional: loop)
            }
        }
    }
}
