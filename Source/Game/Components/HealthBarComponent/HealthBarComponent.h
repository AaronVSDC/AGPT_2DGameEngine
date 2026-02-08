#ifndef HEALTH_BAR_COMPONENT_H
#define HEALTH_BAR_COMPONENT_H

#include "BaseComponent.h"

namespace Papyrus { class TextureComponent; }

namespace xc
{
    class HealthComponent;

    class HealthBarComponent final : public Papyrus::BaseComponent
    {
    public:
        HealthBarComponent() = default;

        void update(float dt) override;
    private:
        HealthComponent* m_HealthComponent = nullptr;
        float m_OriginalWidth = 0.0f;

        Papyrus::TextureComponent* m_TextureComponent = nullptr;
        

        
    };
}

#endif
