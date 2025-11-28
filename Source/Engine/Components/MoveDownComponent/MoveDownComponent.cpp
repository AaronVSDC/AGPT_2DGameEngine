#include "MoveDownComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"

namespace Papyrus
{
    MoveDownComponent::MoveDownComponent(float speed)
        : m_speed(speed)
    {
    }

    void MoveDownComponent::update(float deltaTime)
    {
        auto owner = getOwner();
        if (!owner) return;

        owner->m_Transform.position.y += m_speed * deltaTime;

        // Optional: reset to top if it goes off screen (for looping)
        auto texComp = owner->getComponent<TextureComponent>();
        if (texComp)
        {
            float textureHeight = texComp->getSize().y;
            if (owner->m_Transform.position.y > 720.f) // window height
                owner->m_Transform.position.y = -textureHeight;
        }
    }
}
