#include "PhysicsManager.h"
#include "GameObject.h"

namespace Papyrus
{
    PhysicsManager::PhysicsManager()
    {
        b2WorldDef worldDefinition = b2DefaultWorldDef();
        worldDefinition.gravity = b2Vec2{ 0.0f, 0.0f };
        m_worldId = b2CreateWorld(&worldDefinition);
    }

    PhysicsManager::~PhysicsManager()
    {
        if (b2World_IsValid(m_worldId))
        {
            b2DestroyWorld(m_worldId);
            m_worldId = {};
        }
    }

    void PhysicsManager::fixedUpdate(float fixedDeltaTime)
    {
        if (!b2World_IsValid(m_worldId))
            return;

        //clear previous step overlaps
        m_overlapBeginEventsThisStep.clear(); 

        //step physics
        b2World_Step(m_worldId, fixedDeltaTime, m_subStepCount);

        //collect overlap events from sensors (basic collision detection)
        collectSensorOverlapBeginEvents();
    }

    void PhysicsManager::collectSensorOverlapBeginEvents()
    {
        const b2SensorEvents sensorEvents = b2World_GetSensorEvents(m_worldId);

        for (int beginIndex = 0; beginIndex < sensorEvents.beginCount; ++beginIndex)
        {
            const b2SensorBeginTouchEvent& sensorBeginTouchEvent = sensorEvents.beginEvents[beginIndex];

            GameObject* firstGameObject =
                static_cast<GameObject*>(b2Shape_GetUserData(sensorBeginTouchEvent.sensorShapeId));

            GameObject* secondGameObject =
                static_cast<GameObject*>(b2Shape_GetUserData(sensorBeginTouchEvent.visitorShapeId));

            if (firstGameObject == nullptr || secondGameObject == nullptr)
                continue;

            OverlapBeginEvent overlapBeginEvent{}; 
            overlapBeginEvent.firstGameObject = firstGameObject;
            overlapBeginEvent.secondGameObject = secondGameObject;

            m_overlapBeginEventsThisStep.push_back(overlapBeginEvent);
        }
    }

    std::vector<OverlapBeginEvent> PhysicsManager::consumeOverlapBeginEvents()
    {
        std::vector<OverlapBeginEvent> events = std::move(m_overlapBeginEventsThisStep);
        m_overlapBeginEventsThisStep.clear();
        return events;
    }
}
