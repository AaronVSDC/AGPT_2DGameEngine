#ifndef MOVE_DOWN_COMPONENT_H
#define MOVE_DOWN_COMPONENT_H

#include "BaseComponent.h"
#include <string>

namespace Papyrus
{
    class MoveVerticalComponent : public BaseComponent
    {
    public:
        MoveVerticalComponent();  

        void update(float deltaTime) override;

    private:

    };
}

#endif
