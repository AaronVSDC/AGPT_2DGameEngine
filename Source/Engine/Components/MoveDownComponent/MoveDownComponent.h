#ifndef MOVE_DOWN_COMPONENT_H
#define MOVE_DOWN_COMPONENT_H

#include "BaseComponent.h"

namespace Papyrus
{
    class MoveDownComponent : public BaseComponent
    {
    public:
        MoveDownComponent(float speed);

        void update(float deltaTime) override;

    private:
        float m_speed = 100.f; // pixels per second
    };
}

#endif
