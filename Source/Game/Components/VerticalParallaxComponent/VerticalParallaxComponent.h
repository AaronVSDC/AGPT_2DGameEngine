#ifndef VERTICAL_PARALLAX_COMPONENT_H
#define VERTICAL_PARALLAX_COMPONENT_H

#include "BaseComponent.h"

namespace xc 
{
    class VerticalParallaxComponent : public Papyrus::BaseComponent
    {
    public:
        VerticalParallaxComponent(float speed);

        void update(float deltaTime) override;

    private:
        float m_speed = 100.f; // pixels per second
    };
}

#endif
