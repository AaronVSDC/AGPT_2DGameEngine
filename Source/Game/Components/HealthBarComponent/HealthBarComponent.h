#ifndef HEALTH_BAR_COMPONENT_H
#define HEALTH_BAR_COMPONENT_H

#include "BaseComponent.h"

namespace Papyrus { class GameObject; }

namespace xc
{
    class HealthComponent;

    class HealthBarComponent final : public Papyrus::BaseComponent
    {
    public:
        HealthBarComponent() = default;

        void setBar(Papyrus::GameObject* bar) { m_Bar = bar; }

        void update(float dt) override;
    private:
        HealthComponent* m_HealthComponent = nullptr;
        Papyrus::GameObject* m_Bar = nullptr; // texture
        float m_OriginalWidth = 0.0f;
    };
}

#endif
