#include "ExplosionComponent.h"
#include "TextureComponent.h"
#include "GameObject.h"
#include "Renderer.h"

namespace xc
{
    ExplosionComponent::ExplosionComponent(int columns, int rows, int frameCount, float framesPerSecond)
        : m_columns(columns)
        , m_rows(rows)
        , m_frameCount(frameCount)
        , m_framesPerSecond(framesPerSecond)
    {
    }

    void ExplosionComponent::start()
    {
        m_textureComponent = getOwner()->getComponent<TextureComponent>();

        const b2Vec2 textureSize = m_textureComponent->getSize();

        m_frameWidthPixels = static_cast<int>(textureSize.x / m_columns);
        m_frameHeightPixels = static_cast<int>(textureSize.y / m_rows);
    }

    void ExplosionComponent::update(float deltaTime)
    {
        m_timeAccumulator += deltaTime;
        const float secondsPerFrame = 1.0f / m_framesPerSecond;

        if (m_timeAccumulator < secondsPerFrame)
            return;

        m_timeAccumulator = 0.0f;
        ++m_currentFrame;

        if (m_currentFrame >= m_frameCount)
        {
            getOwner()->markForRemoval();   
        }
    }

    void ExplosionComponent::render() const
    {
        if (!m_textureComponent)
            return;

        const int column = m_currentFrame % m_columns;
        const int row = m_currentFrame / m_columns;

        SDL_FRect source{
            static_cast<float>(column * m_frameWidthPixels),
            static_cast<float>(row * m_frameHeightPixels),
            static_cast<float>(m_frameWidthPixels),
            static_cast<float>(m_frameHeightPixels)
        };

        const auto pos = getOwner()->m_Transform.position;

        Renderer::getInstance().renderTexture(
            *m_textureComponent->getTexture(),
            source,
            pos.x,
            pos.y,
            static_cast<float>(m_frameWidthPixels),
            static_cast<float>(m_frameHeightPixels),
            0.0f
        );
    }
}
