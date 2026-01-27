#include "VerticalParallaxComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"

namespace xc
{
    VerticalParallaxComponent::VerticalParallaxComponent(float speed)
        : m_speed(speed)
    {
    }

    void VerticalParallaxComponent::update(float deltaTime)
    {
        auto owner = getOwner();
        if (!owner) return;

        auto texComp = owner->getComponent<Papyrus::TextureComponent>();
        if (!texComp) return;

        float textureHeight = texComp->getSize().y;

        // Move down
        owner->m_Transform.position.y += m_speed * deltaTime;

        // Loop: if the texture goes below the screen, reset it above
        if (owner->m_Transform.position.y >= 480.f) // window height
        {
            owner->m_Transform.position.y = -textureHeight + (owner->m_Transform.position.y - 480.f);
        }
    }
}
