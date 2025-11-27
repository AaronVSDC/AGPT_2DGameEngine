#ifndef PLAYER_ANIMATION_COMPONENT_H
#define PLAYER_ANIMATION_COMPONENT_H

#include "BaseComponent.h"
#include <SDL3/SDL_rect.h>

namespace Papyrus
{
    class TextureComponent;
    class MoveComponent;

    class PlayerAnimationComponent final : public BaseComponent
    {
    public:
        PlayerAnimationComponent(
            int columns,
            float framesPerSecond
        );

        void start() override;
        void update(float deltaTime) override;
        void render() const override;

    private:
        TextureComponent* m_textureComponent = nullptr;
        MoveComponent* m_moveComponent = nullptr;

        int m_columns = 1;
        int m_frameWidthPixels = 0;
        int m_frameHeightPixels = 0;

        int m_totalFrameCount = 0;
        int m_idleFrameIndex = 0;

        float m_framesPerSecond = 0.0f;
        float m_timeAccumulator = 0.0f;

        int m_currentFrameIndex = 0;
    };
}

#endif
