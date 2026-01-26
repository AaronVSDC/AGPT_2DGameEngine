#include "MoveVerticalComponent.h"
#include "GameObject.h"

namespace xc
{
    MoveVerticalComponent::MoveVerticalComponent()
    {
    }

    void MoveVerticalComponent::update(float deltaTime)
    {
        auto owner = getOwner(); 
        if (!owner) return;
        
        float speed = 90.f;
        owner->m_Transform.position.y += speed * deltaTime; 




    }
}
