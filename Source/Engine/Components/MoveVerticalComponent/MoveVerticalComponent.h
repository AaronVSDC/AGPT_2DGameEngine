#ifndef MOVE_DOWN_COMPONENT_H
#define MOVE_DOWN_COMPONENT_H

#include "BaseComponent.h"
#include <string>

namespace Papyrus
{
    class MoveVerticalComponent : public BaseComponent
    {
    public:
        MoveVerticalComponent(float speed, float reverseDistance = 300.f); // optional distance to reverse

        void update(float deltaTime) override;

    private:
        float m_speed = 100.f;
        float m_reverseDistance = 300.f; // distance before reversing
        float m_startY = 0.f;
        bool m_movingDown = true;
    };
}

#endif
