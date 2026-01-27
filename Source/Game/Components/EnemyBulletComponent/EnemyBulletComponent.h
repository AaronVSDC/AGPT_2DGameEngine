#ifndef ENEMY_BULLET_COMPONENT_H
#define ENEMY_BULLET_COMPONENT_H

#include "BaseComponent.h"

namespace xc
{
    class EnemyBulletComponent final : public Papyrus::BaseComponent
    {
    public:
        explicit EnemyBulletComponent(float speedPixelsPerSecond);

        void fixedUpdate(float fixedDeltaTime) override;
        void onTriggerEnter(Papyrus::GameObject* other) override; 

    private:
        float m_speed = 0.0f;
    };
}

#endif
