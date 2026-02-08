#include "HealthBarComponent.h"
#include "HealthComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"
namespace xc
{
    void HealthBarComponent::update(float)
    {
        if (!m_HealthComponent)
        {
            m_HealthComponent = getOwner()->getComponent<HealthComponent>();
        }

        if (!m_HealthComponent || !m_Bar)
            return;

    }
}