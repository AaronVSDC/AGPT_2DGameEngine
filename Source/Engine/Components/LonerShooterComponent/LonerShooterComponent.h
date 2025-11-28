#ifndef LONER_SHOOTER_COMPONENT_H
#define LONER_SHOOTER_COMPONENT_H

#include "BaseComponent.h"
#include <string>

namespace Papyrus
{
    class LonerShooterComponent final : public BaseComponent
    {
    public:
        LonerShooterComponent(
            float shotsPerSecond,
            float bulletSpeedPixelsPerSecond,
            std::string bulletTexturePath
        );

        void update(float deltaTime) override;

    private:
        void spawnBullet();

        float m_shotsPerSecond = 1.0f;
        float m_bulletSpeed = 0.0f;
        std::string m_bulletTexturePath;

        float m_cooldownSeconds = 0.0f;
        float m_timeUntilNextShot = 0.0f;
    };
}

#endif
