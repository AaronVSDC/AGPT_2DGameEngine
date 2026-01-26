#include "MoveHorizontalComponent.h"
#include "GameObject.h"

namespace xc
{
    MoveHorizontalComponent::MoveHorizontalComponent()
    {
    }

    void MoveHorizontalComponent::update(float deltaTime)
    {
        auto owner = getOwner();
        if (!owner) return;

        float speed = 90.f; 
        owner->m_Transform.position.x += speed * deltaTime; 
    }
}
