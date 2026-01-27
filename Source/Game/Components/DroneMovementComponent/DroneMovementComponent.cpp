#include "DroneMovementComponent.h"
#include "GameObject.h"
#include <cmath>

namespace xc
{
    DroneMovementComponent::DroneMovementComponent(float horizontalAmplitude, float verticalSpeed, float phase)
        : m_horizontalAmplitude(horizontalAmplitude)
        , m_verticalSpeed(verticalSpeed)
        , m_phase(phase)
    {
        m_frequency = 1.0f;  // slower wave
    }

    void DroneMovementComponent::update(float deltaTime)
    {
        auto owner = getOwner();
        if (!owner) return;

        auto& pos = owner->m_Transform.position;

        if (!m_initialized)
        {
            m_startX = pos.x;
            m_startY = pos.y;
            m_initialized = true;
        }

        pos.y += m_verticalSpeed * deltaTime; 

        pos.x = m_startX + m_horizontalAmplitude * std::sin((pos.y + m_phase) * 0.02f * m_frequency);
    }
}
