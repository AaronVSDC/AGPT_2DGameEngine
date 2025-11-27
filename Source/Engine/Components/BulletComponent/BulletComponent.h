#ifndef BULLET_COMPONENT_H
#define BULLET_COMPONENT_H

#include "BaseComponent.h"

namespace Papyrus
{
    class BulletComponent final : public BaseComponent
    {
    public:
        explicit BulletComponent(float speedPixelsPerSecond);

        void fixedUpdate(float deltaTime) override;
        void onTriggerEnter(GameObject* other) override; 


    private:
        float m_speed = 0.0f;
    };
}

#endif
