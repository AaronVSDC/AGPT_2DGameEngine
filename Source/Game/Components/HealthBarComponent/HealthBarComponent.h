#ifndef HEALTH_BAR_COMPONENT_H
#define HEALTH_BAR_COMPONENT_H

#include "BaseComponent.h"

namespace Papyrus { 
    class TextureComponent;
    class GameObject; 
}

namespace xc
{
    class HealthComponent;

    class HealthBarComponent final : public Papyrus::BaseComponent
    {
    public:
        HealthBarComponent() = default;

        void start() override;
        void update(float dt) override;
        void render() const override; 
    private:
        HealthComponent* m_HealthComponent = nullptr;
        Papyrus::GameObject* m_Player = nullptr; 
        float m_OriginalWidth = 0.0f; 
        float m_CurrentWidth = 0.f; 
        float m_NewWidth = 0.f; 
        Papyrus::TextureComponent* m_TextureComponent = nullptr;
        
        float m_OriginalHeight{};

        
    };
}

#endif
