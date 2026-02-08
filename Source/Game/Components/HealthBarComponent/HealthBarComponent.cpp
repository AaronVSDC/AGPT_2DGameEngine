#include "HealthBarComponent.h"
#include "HealthComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "Texture2D.h"
#include "Renderer.h"
#include <SceneManager.h>
#include <Scene.h>

namespace xc
{
    void HealthBarComponent::start()
    {
        m_TextureComponent = getOwner()->getComponent<Papyrus::TextureComponent>();

        auto size = m_TextureComponent->getSize();
        m_OriginalWidth = size.x;
        m_OriginalHeight = size.y;

        m_Player =
            Papyrus::SceneManager::getInstance()
            .getCurrentScene()
            ->findGameObjectByTag("Player");
    }

    void HealthBarComponent::update(float)
    {
        if (!m_HealthComponent)
            m_HealthComponent = m_Player->getComponent<HealthComponent>();
    }

    void HealthBarComponent::render() const
    {
        if (m_HealthComponent->getHealth() <= 0)
            return;
        if (!m_HealthComponent or !m_TextureComponent)
            return;

        float health01 =
            float(m_HealthComponent->getHealth()) /
            float(m_HealthComponent->getMaxHealth());

        health01 = std::clamp(health01, 0.0f, 1.0f);

        float newHeight = m_OriginalHeight * health01;

        auto pos = getOwner()->m_Transform.position;
        float rotation = getOwner()->m_Transform.rotation;

        SDL_FPoint pivot{
            m_OriginalWidth * 0.5f,  
            0.0f                     
        };

  
        
            Papyrus::Renderer::getInstance().renderTexture(
                *m_TextureComponent->getTexture(),
                pos.x,
                pos.y,
                m_OriginalWidth,
                newHeight,
                rotation,
                &pivot
            );
        
    }
}
