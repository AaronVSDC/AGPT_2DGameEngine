#ifndef MOVE_COMPONENT_H
#define MOVE_COMPONENT_H

#include "BaseComponent.h"
#include <box2d/math_functions.h>

namespace Papyrus
{
    class MoveComponent final : public Papyrus::BaseComponent
    {
    public:
        MoveComponent(
            float maxSpeedPixelsPerSecond,
            float accelerationPixelsPerSecondSquared,
            float decelerationPixelsPerSecondSquared
        );

        void update(float deltaTime) override;

        // Called by commands
        void addAcceleration(const b2Vec2& direction); 
        const b2Vec2& getVelocity() const { return m_velocity; }


    private:
        b2Vec2 m_velocity{ 0.0f, 0.0f };

        float m_maxSpeed = 0.0f;
        float m_acceleration = 0.0f;
        float m_deceleration = 0.0f;
    };
}

#endif
