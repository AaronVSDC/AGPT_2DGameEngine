#include "AsteroidSplittingComponent.h"

#include "GameObject.h"
#include "SceneManager.h"
#include "TextureComponent.h"
#include "AnimationComponent.h"
#include "PhysicsBodyComponent.h"
#include "BoxColliderComponent.h"
#include "MoveVerticalComponent.h"

namespace xc
{
    AsteroidSplittingComponent::AsteroidSplittingComponent(
        AsteroidSize size,
        const std::string& nextTexturePath
    )
        : m_size(size)
        , m_nextTexturePath(nextTexturePath)
    {
    }

    void AsteroidSplittingComponent::split()
    {
        auto* owner = getOwner();
        if (!owner) return;

        owner->markForRemoval();

        AsteroidSize nextSize;
        std::string nextTexture;
        int columns, rows, frames;
        float fps;

        if (m_size == AsteroidSize::Large)
        {
            nextSize = AsteroidSize::Medium;
            nextTexture = "Resources/Textures/Asteroid2.bmp";

            columns = 8;
            rows = 3;
            frames = 24;
            fps = 8.0f;
        }
        else if (m_size == AsteroidSize::Medium)
        {
            nextSize = AsteroidSize::Small;
            nextTexture = "Resources/Textures/Asteroid3.bmp";

            columns = 8;    
            rows = 2;       
            frames = 16;     
            fps = 8.0f;    
        }
        else
        {
            return;
        }

        constexpr int count = 3;
        constexpr float spacing = 40.0f;

        const auto& pos = owner->m_Transform.position;

        for (int i = 0; i < count; ++i)
        {
            auto asteroid = std::make_unique<Papyrus::GameObject>();
            asteroid->setTag("Enemy");

            float offsetX = (i - 1) * spacing;
            asteroid->m_Transform.position = { pos.x + offsetX, pos.y };

            asteroid->addComponent(std::make_unique<Papyrus::TextureComponent>(nextTexture));
            asteroid->addComponent(std::make_unique<Papyrus::AnimationComponent>(columns, rows, frames, fps));
            asteroid->addComponent(std::make_unique<Papyrus::PhysicsBodyComponent>());
            asteroid->addComponent(std::make_unique<Papyrus::BoxColliderComponent>());
            asteroid->addComponent(std::make_unique<MoveVerticalComponent>());

            if (nextSize != AsteroidSize::Small)
            {
                asteroid->addComponent(std::make_unique<AsteroidSplittingComponent>(nextSize,"Resources/Textures/Asteroid3.bmp"));
            }

            Papyrus::SceneManager::getInstance().getCurrentScene()->add(std::move(asteroid));
        }
    }
}