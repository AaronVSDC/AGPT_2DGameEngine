#include "LonerShooterComponent.h"

#include "GameObject.h"
#include "SceneManager.h"
#include "TextureComponent.h"
#include "AnimationComponent.h"
#include "PhysicsBodyComponent.h"
#include "BoxColliderComponent.h"
#include "EnemyBulletComponent.h"

namespace xc
{
    LonerShooterComponent::LonerShooterComponent(
        float shotsPerSecond,
        float bulletSpeedPixelsPerSecond,
        std::string bulletTexturePath
    )
        : m_shotsPerSecond(shotsPerSecond)
        , m_bulletSpeed(bulletSpeedPixelsPerSecond)
        , m_bulletTexturePath(std::move(bulletTexturePath))
    {
        m_cooldownSeconds = (m_shotsPerSecond > 0.0f) ? (1.0f / m_shotsPerSecond) : 9999.0f;
        m_timeUntilNextShot = m_cooldownSeconds; // first shot after one interval
    }

    void LonerShooterComponent::update(float deltaTime)
    {
        m_timeUntilNextShot -= deltaTime;
        if (m_timeUntilNextShot > 0.0f)
            return;

        m_timeUntilNextShot = m_cooldownSeconds;
        spawnBullet();
    }

    void LonerShooterComponent::spawnBullet()
    {
        GameObject* enemy = getOwner();

        auto bullet = std::make_unique<GameObject>();
        bullet->setTag("EnemyBullet");

        // Spawn position: bottom-center of the enemy frame (approx)
        const b2Vec2 enemyPos = enemy->m_Transform.position;

        float enemyWidth = 32.0f;
        float enemyHeight = 32.0f;

        if (auto* enemyAnimation = enemy->getComponent<AnimationComponent>())
        {
            enemyWidth = static_cast<float>(enemyAnimation->getFrameWidth());
            enemyHeight = static_cast<float>(enemyAnimation->getFrameHeight());
        }
        else if (auto* enemyTexture = enemy->getComponent<TextureComponent>())
        {
            enemyWidth = enemyTexture->getSize().x;
            enemyHeight = enemyTexture->getSize().y;
        }

        bullet->m_Transform.position = {
            enemyPos.x + enemyWidth * 0.5f,
            enemyPos.y + enemyHeight
        };

        // Components (ORDER matters: things that compute frame sizes should start first)
        bullet->addComponent(std::make_unique<TextureComponent>(m_bulletTexturePath));

        // If your enemy bullet is animated, add AnimationComponent here BEFORE collider/body.
        bullet->addComponent(std::make_unique<AnimationComponent>(8, 1, 8, 12));


        bullet->addComponent(std::make_unique<PhysicsBodyComponent>());

        auto boxCollider = std::make_unique<BoxColliderComponent>();
        bullet->addComponent(std::move(boxCollider));


        bullet->addComponent(std::make_unique<EnemyBulletComponent>(m_bulletSpeed));

        SceneManager::getInstance().getCurrentScene()->add(std::move(bullet));
    }
}
