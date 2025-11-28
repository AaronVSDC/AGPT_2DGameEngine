#include "CollisionSize.h"

#include "GameObject.h"
#include "TextureComponent.h"
#include "AnimationComponent.h"
#include "PlayerAnimationComponent.h"

namespace Papyrus
{
    b2Vec2 getCollisionSizePixels(const GameObject* owner)
    {
        if (!owner)
            return { 0.0f, 0.0f };

        // Prefer PlayerAnimationComponent frame size
        if (auto* playerAnimationComponent =
            const_cast<GameObject*>(owner)->getComponent<PlayerAnimationComponent>()) 
        {
            return {
                static_cast<float>(playerAnimationComponent->getFrameWidth()),
                static_cast<float>(playerAnimationComponent->getFrameHeight())
            };
        }

        // Prefer AnimationComponent frame size
        if (auto* animationComponent =
            const_cast<GameObject*>(owner)->getComponent<AnimationComponent>())
        {
            return {
                static_cast<float>(animationComponent->getFrameWidth()),
                static_cast<float>(animationComponent->getFrameHeight())
            };
        }

        // Fallback: non-spritesheet textures
        if (auto* textureComponent =
            const_cast<GameObject*>(owner)->getComponent<TextureComponent>())
        {
            return textureComponent->getSize();
        }

        return { 0.0f, 0.0f };
    }
}
