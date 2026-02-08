#include "AnimationComponent.h"

#include "TextureComponent.h"
#include "GameObject.h"
#include "Renderer.h"

namespace Papyrus
{
    AnimationComponent::AnimationComponent(
        int columns,
        int rows,
        int totalFrameCount,
        float framesPerSecond,
        bool loop
    )
        : m_columns(columns)
        , m_rows(rows)
        , m_totalFrameCount(totalFrameCount)
        , m_framesPerSecond(framesPerSecond)
        , m_loop(loop)
    {
    }

    void AnimationComponent::start()
    {
        m_textureComponent = getOwner()->getComponent<TextureComponent>();
        if (!m_textureComponent)
            return;

        const b2Vec2 textureSize = m_textureComponent->getSize();

        // Automatic frame size
        m_frameWidthPixels = static_cast<int>(textureSize.x / static_cast<float>(m_columns));
        m_frameHeightPixels = static_cast<int>(textureSize.y / static_cast<float>(m_rows));


    }

    void AnimationComponent::update(float deltaTime)
    {


        if (!m_textureComponent)
            return;

        m_timeAccumulator += deltaTime;

        const float secondsPerFrame = 1.0f / m_framesPerSecond;

        while (m_timeAccumulator >= secondsPerFrame)
        {
            m_timeAccumulator -= secondsPerFrame;
            ++m_currentFrameIndex;

            if (m_currentFrameIndex >= m_totalFrameCount)
            {
                if (m_loop)
                    m_currentFrameIndex = 0;
                else
                    m_currentFrameIndex = m_totalFrameCount - 1;
            }
        }
    }

    void AnimationComponent::render() const
    {
        if (!m_textureComponent)
            return;

        const Texture2D& texture = *m_textureComponent->getTexture();

        // Convert linear frame index ? (column, row)
        const int columnIndex = m_currentFrameIndex % m_columns;
        const int rowIndex = m_currentFrameIndex / m_columns;

        const float sourceX = static_cast<float>(columnIndex * m_frameWidthPixels);
        const float sourceY = static_cast<float>(rowIndex * m_frameHeightPixels);

        SDL_FRect sourceRectangle{
            sourceX,
            sourceY,
            static_cast<float>(m_frameWidthPixels),
            static_cast<float>(m_frameHeightPixels)
        };

        const auto position = getOwner()->m_Transform.position;

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
