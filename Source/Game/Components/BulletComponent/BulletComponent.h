#ifndef BULLET_COMPONENT_H
#define BULLET_COMPONENT_H

#include "BaseComponent.h"

namespace xc
{
    class BulletComponent final : public Papyrus::BaseComponent
    {
    public:
        explicit BulletComponent(float speedPixelsPerSecond);

        void fixedUpdate(float deltaTime) override;
        void onTriggerEnter(Papyrus::GameObject* other) override;


    private:
        float m_speed = 0.0f;
    };
}

#endif
