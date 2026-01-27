#pragma once
#include "BaseComponent.h"

namespace xc
{
    class DroneMovementComponent : public Papyrus::BaseComponent
    {
    public:
        DroneMovementComponent(float horizontalAmplitude = 50.0f, float verticalSpeed = 50.0f, float phase = 0.0f);

        void update(float deltaTime) override;

    private:
        bool m_initialized = false;
        float m_startY = 0.0f;
        float m_startX = 0.0f;

        float m_horizontalAmplitude = 40.0f; // <--Horizontal amplitude of the drones flight
        float m_verticalSpeed = 50.0f;       
        float m_phase = 0.0f;                
        float m_frequency = 2.0f;            
    };
}
