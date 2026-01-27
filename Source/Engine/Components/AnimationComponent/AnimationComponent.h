#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include "BaseComponent.h"
#include <SDL3/SDL_rect.h>

//namespace XenonClone
namespace Papyrus
{
    class TextureComponent;

    class AnimationComponent final : public BaseComponent
    {
    public:
        AnimationComponent(
            int columns,
            int rows, 
            int totalFrameCount,
            float framesPerSecond,
            bool loop = true
        );

        void start() override;
        void update(float deltaTime) override;
        void render() const override;


        int getFrameWidth() const { return m_frameWidthPixels; }
        int getFrameHeight() const { return m_frameHeightPixels; }

    private:
        TextureComponent* m_textureComponent = nullptr;

        int m_columns = 1;
        int m_rows = 1;
        int m_totalFrameCount = 1;

        int m_frameWidthPixels = 0;
        int m_frameHeightPixels = 0;

        float m_framesPerSecond = 0.0f;
        float m_timeAccumulator = 0.0f;

        int m_currentFrameIndex = 0;
        bool m_loop = true;
    };
}

#endif
