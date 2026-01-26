#include "DroneMovementComponent.h"
#include "GameObject.h"

namespace xc
{
    DroneMovementComponent::DroneMovementComponent()
    {
    }

    void DroneMovementComponent::update(float deltaTime)
    {
        auto owner = getOwner();
        if (!owner) return;

        auto& pos = owner->m_Transform.position;

        if (!m_initialized)
        {
            m_originX = pos.x;
            m_initialized = true;
        }

        pos.x += m_direction * m_horizontalSpeed * deltaTime;

        if (pos.x > m_originX + m_range)
            m_direction = -1;    // go left
        else if (pos.x < m_originX - m_range)
            m_direction = 1;     // go right

        pos.y += m_verticalSpeed * deltaTime;
    }


}
