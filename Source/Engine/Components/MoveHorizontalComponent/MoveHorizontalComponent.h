#ifndef MOVE_HORIZONTAL_COMPONENT_H
#define MOVE_HORIZONTAL_COMPONENT_H

#include "BaseComponent.h"

namespace Papyrus
{
    class MoveHorizontalComponent : public BaseComponent
    {
    public:
        MoveHorizontalComponent();

        void update(float deltaTime) override;

    private:

    };
}

#endif
