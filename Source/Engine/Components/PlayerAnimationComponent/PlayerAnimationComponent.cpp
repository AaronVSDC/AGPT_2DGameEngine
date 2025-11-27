#include "PlayerAnimationComponent.h"

#include "TextureComponent.h"
#include "MoveComponent.h"
#include "GameObject.h"
#include "Renderer.h"

#include <cmath>

namespace Papyrus
{
    PlayerAnimationComponent::PlayerAnimationComponent(
        int columns,
        float framesPerSecond
    )
        : m_columns(columns)
        , m_framesPerSecond(framesPerSecond)
    {
    }

    void PlayerAnimationComponent::start()
    {
        m_textureComponent = getOwner()->getComponent<TextureComponent>();
        m_moveComponent = getOwner()->getComponent<MoveComponent>();

        if (!m_textureComponent)
            return;

        const b2Vec2 textureSize = m_textureComponent->getSize();

        m_totalFrameCount = m_columns;
        m_idleFrameIndex = m_totalFrameCount / 2;

        m_frameWidthPixels = static_cast<int>(textureSize.x / static_cast<float>(m_columns));
        m_frameHeightPixels = static_cast<int>(textureSize.y);

        m_currentFrameIndex = m_idleFrameIndex;
    }

    void PlayerAnimationComponent::update(float deltaTime)
    {
        if (!m_textureComponent || !m_moveComponent)
            return;

        const float velocityX = m_moveComponent->getVelocity().x;
        const float deadZone = 0.01f;

        // --------------------
        // IDLE ? snap to middle
        // --------------------
        if (std::abs(velocityX) < deadZone)
        {
            m_currentFrameIndex = m_idleFrameIndex;
            m_timeAccumulator = 0.0f;
            return;
        }

        m_timeAccumulator += deltaTime;
        const float secondsPerFrame = 1.0f / m_framesPerSecond;

        if (m_timeAccumulator < secondsPerFrame)
            return;

        m_timeAccumulator = 0.0f;

        // --------------------
        // MOVING RIGHT ? CLAMP AT RIGHT END
        // --------------------
        if (velocityX > 0.0f)
        {
            if (m_currentFrameIndex < m_totalFrameCount - 1)
                ++m_currentFrameIndex;

            return;
        }

        // --------------------
        // MOVING LEFT ? CLAMP AT LEFT END
        // --------------------
        if (velocityX < 0.0f)
        {
            if (m_currentFrameIndex > 0)
                --m_currentFrameIndex;
        }
    }


    void PlayerAnimationComponent::render() const
    {
        if (!m_textureComponent)
            return;

        const Texture2D& texture = *m_textureComponent->getTexture();
        const auto position = getOwner()->m_Transform.position;

        const float sourceX = static_cast<float>(m_currentFrameIndex * m_frameWidthPixels);
        const float sourceY = 0.0f;

        SDL_FRect sourceRectangle{
            sourceX,
            sourceY,
            static_cast<float>(m_frameWidthPixels),
            static_cast<float>(m_frameHeightPixels)
        };

        Renderer::getInstance().renderTexture(
            texture,
            sourceRectangle,
            position.x,
            position.y,
            static_cast<float>(m_frameWidthPixels),
            static_cast<float>(m_frameHeightPixels),
            getOwner()->m_Transform.rotation
        );
    }
}
