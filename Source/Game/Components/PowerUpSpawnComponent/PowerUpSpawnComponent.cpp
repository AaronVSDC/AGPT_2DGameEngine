#include "PowerUpSpawnComponent.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "AnimationComponent.h"
#include "PhysicsBodyComponent.h"
#include "BoxColliderComponent.h"
#include "MoveHorizontalComponent.h" 
#include "MoveVerticalComponent.h"
#include "DroneMovementComponent.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "LonerShooterComponent.h"
#include "AsteroidSplittingComponent.h"

#include <algorithm>

namespace xc
{
    static b2Vec2 getFrameSizePixels(const std::string& texturePath, int columns, int rows)
    {
        auto* texture = Papyrus::ResourceManager::getInstance().loadTexture(texturePath);
        if (!texture) return { 0.0f, 0.0f };

        const b2Vec2 sheetSize = texture->getSize();
        if (columns <= 0 || rows <= 0) return { sheetSize.x, sheetSize.y };

        return { sheetSize.x / static_cast<float>(columns), sheetSize.y / static_cast<float>(rows) };
    }

    PowerUpSpawnComponent::PowerUpSpawnComponent(float spawnIntervalSeconds, float screenWidthPixels, float spawnYTopPixels)
        : m_spawnIntervalSeconds(spawnIntervalSeconds)
        , m_screenWidthPixels(screenWidthPixels)
        , m_spawnYTopPixels(spawnYTopPixels)
        , m_randomEngine(std::random_device{}())
    {
    }

    void PowerUpSpawnComponent::update(float deltaTime)
    {
        m_timeAccumulatorSeconds += deltaTime;

        // Keep it regular even if deltaTime spikes
        while (m_timeAccumulatorSeconds >= m_spawnIntervalSeconds)
        {
            m_timeAccumulatorSeconds -= m_spawnIntervalSeconds;
            spawnRandomPowerUp();
        }
    }

    float PowerUpSpawnComponent::randomFloat(float minValue, float maxValue)
    {
        if (maxValue <= minValue) return minValue;

        std::uniform_real_distribution<float> distribution(minValue, maxValue);
        return distribution(m_randomEngine);
    }

    void PowerUpSpawnComponent::spawnRandomPowerUp()
    {
        const int pick = m_enemyPickDistribution(m_randomEngine);

        switch (pick)
        {
        case 0:
            spawnShield();
            break;

        case 1:
            spawnWeapon();
            break;

        default:
            spawnShield();
            break;
        }
    }


    void PowerUpSpawnComponent::spawnShield()
    {
        constexpr const char* texturePath = "Resources/Textures/PUShield.bmp";
        constexpr int columns = 4;
        constexpr int rows = 2;
        constexpr int frames = 8;
        constexpr float framesPerSecond = 8.0f;

        const b2Vec2 frameSize = getFrameSizePixels(texturePath, columns, rows);
        const float spawnX = randomFloat(0.0f, std::max(0.0f, m_screenWidthPixels - frameSize.x));
        const float spawnY = m_spawnYTopPixels - frameSize.y;

        auto powerUp = std::make_unique<Papyrus::GameObject>();
        powerUp->setTag("PowerUp");
        powerUp->m_Transform.position = { spawnX, spawnY };

        powerUp->addComponent(std::make_unique<Papyrus::TextureComponent>(texturePath));
        powerUp->addComponent(std::make_unique<Papyrus::AnimationComponent>(columns, rows, frames, framesPerSecond));
        powerUp->addComponent(std::make_unique<Papyrus::PhysicsBodyComponent>());
        powerUp->addComponent(std::make_unique<Papyrus::BoxColliderComponent>());
        powerUp->addComponent(std::make_unique<MoveVerticalComponent>());

        Papyrus::SceneManager::getInstance().getCurrentScene()->add(std::move(powerUp));
    }

    void PowerUpSpawnComponent::spawnWeapon()
    {
        constexpr const char* texturePath = "Resources/Textures/PUWeapon.bmp";
        constexpr int columns = 4;
        constexpr int rows = 2;
        constexpr int frames = 8;
        constexpr float framesPerSecond = 8.0f;

        const b2Vec2 frameSize = getFrameSizePixels(texturePath, columns, rows);
        const float spawnX = randomFloat(0.0f, std::max(0.0f, m_screenWidthPixels - frameSize.x));
        const float spawnY = m_spawnYTopPixels - frameSize.y;

        auto powerUp = std::make_unique<Papyrus::GameObject>();
        powerUp->setTag("PowerUp");
        powerUp->m_Transform.position = { spawnX, spawnY };

        powerUp->addComponent(std::make_unique<Papyrus::TextureComponent>(texturePath));
        powerUp->addComponent(std::make_unique<Papyrus::AnimationComponent>(columns, rows, frames, framesPerSecond));
        powerUp->addComponent(std::make_unique<Papyrus::PhysicsBodyComponent>());
        powerUp->addComponent(std::make_unique<Papyrus::BoxColliderComponent>());
        powerUp->addComponent(std::make_unique<MoveVerticalComponent>());

        Papyrus::SceneManager::getInstance().getCurrentScene()->add(std::move(powerUp));
    }
}