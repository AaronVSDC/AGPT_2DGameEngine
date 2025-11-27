#include "MoveComponent.h"
#include "GameObject.h"
#include <algorithm>
#include <cmath>

namespace Papyrus
{
    MoveComponent::MoveComponent(
        float maxSpeedPixelsPerSecond,
        float accelerationPixelsPerSecondSquared,
        float decelerationPixelsPerSecondSquared
    )
        : m_maxSpeed(maxSpeedPixelsPerSecond)
        , m_acceleration(accelerationPixelsPerSecondSquared)
        , m_deceleration(decelerationPixelsPerSecondSquared)
    {
    }

    void MoveComponent::addAcceleration(const b2Vec2& direction)
    {
        // direction must be normalized (1,0), (-1,0), (0,1), (0,-1)
        m_velocity += direction * m_acceleration;
    }

    void MoveComponent::update(float deltaTime)
    {
        // --- Apply deceleration (friction) ---
        const float speed = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);

        if (speed > 0.0f)
        {
            const float decelAmount = m_deceleration * deltaTime;
            const float newSpeed = std::max(0.0f, speed - decelAmount);

            if (newSpeed == 0.0f)
            {
                m_velocity = { 0.0f, 0.0f };
            }
            else
            {
                const float scale = newSpeed / speed;
                m_velocity.x *= scale;
                m_velocity.y *= scale;
            }
        }

        // --- Clamp max speed ---
        const float clampedSpeed = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
        if (clampedSpeed > m_maxSpeed)
        {
            const float scale = m_maxSpeed / clampedSpeed;
            m_velocity.x *= scale;
            m_velocity.y *= scale;
        }

        // --- Integrate position ---
        GameObject* owner = getOwner();
        owner->m_Transform.position += m_velocity * deltaTime;
    }
}
