#include "ExplosionUtility.h"

#include "GameObject.h"
#include "TextureComponent.h"
#include "AnimationComponent.h"
#include "PlayerAnimationComponent.h"
#include "MoveComponent.h"
#include "ExplosionComponent.h"
#include "BoxColliderComponent.h"

namespace Papyrus
{
    void explodeAndDie(
        GameObject* target,
        const std::string& explosionTexturePath,
        int columns,
        int rows,
        int frames,
        float framesPerSecond
    )
    {
        if (!target) return;

        // Stop normal visuals / movement
        if (auto* playerAnim = target->getComponent<PlayerAnimationComponent>())
            playerAnim->m_Enabled = false;

        if (auto* anim = target->getComponent<AnimationComponent>())
            anim->m_Enabled = false;

        if (auto* move = target->getComponent<MoveComponent>())
            move->m_Enabled = false;

        // Optional: stop further hits while exploding
        if (auto* collider = target->getComponent<BoxColliderComponent>())
            collider->m_Enabled = false;

        // Swap texture on the existing TextureComponent (don’t add a second one)
        if (auto* texture = target->getComponent<TextureComponent>())
        {
            texture->setTexture(explosionTexturePath);
        }
        else
        {
            target->addComponent(std::make_unique<TextureComponent>(explosionTexturePath));
        }

        // Add one-shot explosion animator
        auto explosion = std::make_unique<ExplosionComponent>(columns, rows, frames, framesPerSecond);
        ExplosionComponent* explosionRawPointer = explosion.get();
        target->addComponent(std::move(explosion));

        // Start only the new component (don’t call target->start() again)
        explosionRawPointer->start();
        explosionRawPointer->onEnable();
    }
}
