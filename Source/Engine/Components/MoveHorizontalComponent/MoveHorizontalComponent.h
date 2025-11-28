#ifndef MOVE_HORIZONTAL_COMPONENT_H
#define MOVE_HORIZONTAL_COMPONENT_H

#include "BaseComponent.h"

namespace Papyrus
{
    class MoveHorizontalComponent : public BaseComponent
    {
    public:
        MoveHorizontalComponent(float speed, float reverseDistance = 200.f);

        void update(float deltaTime) override;

    private:
        float m_speed = 100.f;           // pixels per second
        float m_reverseDistance = 200.f;  // distance to reverse
        float m_startX = 0.f;
        bool m_movingLeft = true;        // start moving left
    };
}

#endif
