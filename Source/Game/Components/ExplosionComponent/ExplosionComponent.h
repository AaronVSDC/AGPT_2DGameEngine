#ifndef EXPLOSION_COMPONENT_H
#define EXPLOSION_COMPONENT_H

#include "BaseComponent.h"

namespace Papyrus
{
    class TextureComponent; 
}
namespace xc
{


    class ExplosionComponent final : public Papyrus::BaseComponent
    {
    public:
        ExplosionComponent(int columns, int rows, int frameCount, float framesPerSecond);

        void start() override;
        void update(float deltaTime) override;
        void render() const override;

    private:
        Papyrus::TextureComponent* m_textureComponent = nullptr;

        int m_columns = 1;
        int m_rows = 1;
        int m_frameCount = 1;

        int m_frameWidthPixels = 0;
        int m_frameHeightPixels = 0;

        int m_currentFrame = 0;

        float m_framesPerSecond = 0.0f;
        float m_timeAccumulator = 0.0f;
    };
}

#endif
