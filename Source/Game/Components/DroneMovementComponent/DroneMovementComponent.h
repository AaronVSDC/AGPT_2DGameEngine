#ifndef DRONE_MOVEMENT_COMPONENT_H
#define DRONE_MOVEMENT_COMPONENT_H

#include "BaseComponent.h"
#include <string>

namespace xc
{
    class DroneMovementComponent : public Papyrus::BaseComponent
    {
    public:
        DroneMovementComponent();

        void update(float deltaTime) override;
        float m_horizontalSpeed = 50.f;
        float m_verticalSpeed = 90.f;
        float m_range = 40.f;
        int m_direction = 1;
        float m_originX = 0.f;
        bool m_initialized = false;

    private:

    };
}

#endif
