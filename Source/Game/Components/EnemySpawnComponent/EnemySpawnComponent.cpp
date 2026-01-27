#include "EnemySpawnComponent.h"

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

    EnemySpawnerComponent::EnemySpawnerComponent(float spawnIntervalSeconds, float screenWidthPixels, float spawnYTopPixels)
        : m_spawnIntervalSeconds(spawnIntervalSeconds)
        , m_screenWidthPixels(screenWidthPixels)
        , m_spawnYTopPixels(spawnYTopPixels) 
        , m_randomEngine(std::random_device{}())
    {
    }

    void EnemySpawnerComponent::update(float deltaTime)
    {
        m_timeAccumulatorSeconds += deltaTime;

        // Keep it regular even if deltaTime spikes
        while (m_timeAccumulatorSeconds >= m_spawnIntervalSeconds)
        {
            m_timeAccumulatorSeconds -= m_spawnIntervalSeconds;
            spawnRandomEnemy();
        }
    }

    float EnemySpawnerComponent::randomFloat(float minValue, float maxValue)
    {
        if (maxValue <= minValue) return minValue;

        std::uniform_real_distribution<float> distribution(minValue, maxValue);
        return distribution(m_randomEngine);
    }

    void EnemySpawnerComponent::spawnRandomEnemy()
    {
        const int pick = m_enemyPickDistribution(m_randomEngine);

        switch (pick)
        {
        case 0:
            spawnLoner();
            break;

        case 1:
            spawnRusher();
            break;

        case 2:
            spawnDrone();
            break;
        case 3:
            spawnAesteroid();
            break;

        case 4:
            spawnMetalAesteroid();
            break;

        default:
            spawnRusher();
            break;
        }
    }


    void EnemySpawnerComponent::spawnLoner()
    {
        constexpr const char* texturePath = "Resources/Textures/LonerA.bmp";
        constexpr int columns = 4;
        constexpr int rows = 4;
        constexpr int frames = 16;
        constexpr float framesPerSecond = 8.0f;

        const b2Vec2 frameSize = getFrameSizePixels(texturePath, columns, rows);
        const float spawnX = -frameSize.x;
        const float spawnY = randomFloat(frameSize.y , 100.f);

        auto enemy = std::make_unique<Papyrus::GameObject>();
        enemy->setTag("Enemy");
        enemy->m_Transform.position = { spawnX, spawnY };

        enemy->addComponent(std::make_unique<Papyrus::TextureComponent>(texturePath));
        enemy->addComponent(std::make_unique<Papyrus::AnimationComponent>(columns, rows, frames, framesPerSecond));
        enemy->addComponent(std::make_unique<Papyrus::PhysicsBodyComponent>());
        enemy->addComponent(std::make_unique<Papyrus::BoxColliderComponent>());
        enemy->addComponent(std::make_unique<MoveHorizontalComponent>());
        enemy->addComponent(std::make_unique<LonerShooterComponent>(1.2f, 100.f, "Resources/Textures/EnWeap6.bmp"));

        Papyrus::SceneManager::getInstance().getCurrentScene()->add(std::move(enemy));
    }

    void EnemySpawnerComponent::spawnRusher()
    {
        constexpr const char* texturePath = "Resources/Textures/rusher.bmp";
        constexpr int columns = 4;
        constexpr int rows = 6;
        constexpr int frames = 24;
        constexpr float framesPerSecond = 8.0f;

        const b2Vec2 frameSize = getFrameSizePixels(texturePath, columns, rows);
        const float spawnX = randomFloat(0.0f, std::max(0.0f, m_screenWidthPixels - frameSize.x));
        const float spawnY = m_spawnYTopPixels - frameSize.y;

        auto enemy = std::make_unique<Papyrus::GameObject>();
        enemy->setTag("Enemy");
        enemy->m_Transform.position = { spawnX, spawnY };

        enemy->addComponent(std::make_unique<Papyrus::TextureComponent>(texturePath));
        enemy->addComponent(std::make_unique<Papyrus::AnimationComponent>(columns, rows, frames, framesPerSecond));
        enemy->addComponent(std::make_unique<Papyrus::PhysicsBodyComponent>());
        enemy->addComponent(std::make_unique<Papyrus::BoxColliderComponent>());
        enemy->addComponent(std::make_unique<MoveVerticalComponent>()); 

        Papyrus::SceneManager::getInstance().getCurrentScene()->add(std::move(enemy));
    }

    void EnemySpawnerComponent::spawnDrone()
    {
        constexpr const char* texturePath = "Resources/Textures/drone.bmp";
        constexpr int columns = 8;
        constexpr int rows = 2;
        constexpr int frames = 16;
        constexpr float framesPerSecond = 8.0f;

        const b2Vec2 frameSize = getFrameSizePixels(texturePath, columns, rows);
        const float spawnX = randomFloat(0.0f, std::max(0.0f, m_screenWidthPixels - frameSize.x));
        const float spawnY = m_spawnYTopPixels - frameSize.y;

        const float verticalSpacing = 40.0f;

        for (int i = 0; i < 5; ++i)
        {
            auto enemy = std::make_unique<Papyrus::GameObject>();
            enemy->setTag("Enemy");

            enemy->m_Transform.position = { spawnX, spawnY - (i * verticalSpacing) };

            enemy->addComponent(std::make_unique<Papyrus::TextureComponent>(texturePath));
            enemy->addComponent(std::make_unique<Papyrus::AnimationComponent>(columns, rows, frames, framesPerSecond));
            enemy->addComponent(std::make_unique<Papyrus::PhysicsBodyComponent>());
            enemy->addComponent(std::make_unique<Papyrus::BoxColliderComponent>());
            enemy->addComponent(std::make_unique<DroneMovementComponent>());

            Papyrus::SceneManager::getInstance().getCurrentScene()->add(std::move(enemy));
        }
    }
    void EnemySpawnerComponent::spawnAesteroid()
    {
        constexpr const char* texturePath = "Resources/Textures/Aesteroid.bmp";
        constexpr int columns = 5;
        constexpr int rows = 5;
        constexpr int frames = 25;
        constexpr float framesPerSecond = 8.0f;

        const b2Vec2 frameSize = getFrameSizePixels(texturePath, columns, rows);
        const float spawnX = randomFloat(0.0f,std::max(0.0f, m_screenWidthPixels - frameSize.x));
        const float spawnY = m_spawnYTopPixels - frameSize.y;

        auto enemy = std::make_unique<Papyrus::GameObject>();
        enemy->setTag("Enemy");

        enemy->m_Transform.position = { spawnX, spawnY };

        enemy->addComponent(std::make_unique<Papyrus::TextureComponent>(texturePath));
        enemy->addComponent(std::make_unique<Papyrus::AnimationComponent>(columns, rows, frames, framesPerSecond));
        enemy->addComponent(std::make_unique<Papyrus::PhysicsBodyComponent>());
        enemy->addComponent(std::make_unique<Papyrus::BoxColliderComponent>());
        enemy->addComponent(std::make_unique<MoveVerticalComponent>());

        //Split
        enemy->addComponent(std::make_unique<AsteroidSplittingComponent>(AsteroidSize::Large,"Resources/Textures/Asteroid2.bmp"));

        Papyrus::SceneManager::getInstance().getCurrentScene()->add(std::move(enemy));
    }

    void EnemySpawnerComponent::spawnMetalAesteroid()
    {
        struct AsteroidData
        {
            const char* texturePath;
            int columns;
            int rows;
            int frames;
            float framesPerSecond;
        };

        const AsteroidData asteroids[3] =
        {   //columns, rows, frames, fps 
            { "Resources/Textures/MetalAesteroid.bmp", 5, 5, 25, 8.0f },
            { "Resources/Textures/MetalAesteroid2.bmp", 8, 3, 24, 8.0f },
            { "Resources/Textures/MetalAesteroid3.bmp", 8, 2, 16, 8.0f }
        };

        // Pick a random asteroid
        std::uniform_int_distribution<int> dist(0, 2);
        const AsteroidData& choice = asteroids[dist(m_randomEngine)];

        const b2Vec2 frameSize = getFrameSizePixels(choice.texturePath, choice.columns, choice.rows);
        const float spawnX = randomFloat(0.0f, std::max(0.0f, m_screenWidthPixels - frameSize.x));
        const float spawnY = m_spawnYTopPixels - frameSize.y;

        auto enemy = std::make_unique<Papyrus::GameObject>();
        enemy->setTag("Indestructible");
        enemy->m_Transform.position = { spawnX, spawnY };

        enemy->addComponent(std::make_unique<Papyrus::TextureComponent>(choice.texturePath));
        enemy->addComponent(std::make_unique<Papyrus::AnimationComponent>(choice.columns, choice.rows, choice.frames, choice.framesPerSecond));
        enemy->addComponent(std::make_unique<Papyrus::PhysicsBodyComponent>());
        enemy->addComponent(std::make_unique<Papyrus::BoxColliderComponent>());
        enemy->addComponent(std::make_unique<MoveVerticalComponent>());

        Papyrus::SceneManager::getInstance().getCurrentScene()->add(std::move(enemy));
    }



}
