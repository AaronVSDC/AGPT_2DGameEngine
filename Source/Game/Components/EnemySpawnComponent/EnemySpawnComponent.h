#ifndef ENEMY_SPAWNER_COMPONENT_H
#define ENEMY_SPAWNER_COMPONENT_H

#include "BaseComponent.h"
#include <random>
#include <string>

namespace xc
{
    class EnemySpawnerComponent final : public BaseComponent
    {
    public:
        EnemySpawnerComponent(
            float spawnIntervalSeconds,
            float screenWidthPixels,
            float spawnYTopPixels = -10.0f
        );

        void update(float deltaTime) override;

    private:
        void spawnRandomEnemy();
        void spawnLoner();
        void spawnRusher();
        void spawnDrone();

        float randomFloat(float minValue, float maxValue);

    private:
        float m_spawnIntervalSeconds = 1.0f;
        float m_timeAccumulatorSeconds = 0.0f;

        float m_screenWidthPixels = 640.0f;
        float m_spawnYTopPixels = -10.0f;

        std::mt19937 m_randomEngine;
        std::uniform_int_distribution<int> m_enemyPickDistribution{ 0, 2 };
    };
}

#endif
