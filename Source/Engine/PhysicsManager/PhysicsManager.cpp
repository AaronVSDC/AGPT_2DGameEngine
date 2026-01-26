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

    static void dispatchSensorEventsToComponents(b2WorldId worldId)
    {
        const b2SensorEvents sensorEvents = b2World_GetSensorEvents(worldId);

        for (int beginIndex = 0; beginIndex < sensorEvents.beginCount; ++beginIndex)
        {
            const b2SensorBeginTouchEvent& sensorBeginTouchEvent = sensorEvents.beginEvents[beginIndex];

            GameObject* sensorGameObject =
                static_cast<GameObject*>(b2Shape_GetUserData(sensorBeginTouchEvent.sensorShapeId));

            GameObject* visitorGameObject =
                static_cast<GameObject*>(b2Shape_GetUserData(sensorBeginTouchEvent.visitorShapeId));

            if (sensorGameObject && visitorGameObject)
            {
                sensorGameObject->notifyTriggerEnter(visitorGameObject);
                visitorGameObject->notifyTriggerEnter(sensorGameObject);
            }
        }

        for (int endIndex = 0; endIndex < sensorEvents.endCount; ++endIndex)
        {
            const b2SensorEndTouchEvent& sensorEndTouchEvent = sensorEvents.endEvents[endIndex];

            if (!b2Shape_IsValid(sensorEndTouchEvent.sensorShapeId) ||
                !b2Shape_IsValid(sensorEndTouchEvent.visitorShapeId))
            {
                continue;
            }

            GameObject* sensorGameObject =
                static_cast<GameObject*>(b2Shape_GetUserData(sensorEndTouchEvent.sensorShapeId));

            GameObject* visitorGameObject =
                static_cast<GameObject*>(b2Shape_GetUserData(sensorEndTouchEvent.visitorShapeId));

            if (sensorGameObject && visitorGameObject)
            {
                sensorGameObject->notifyTriggerExit(visitorGameObject);
                visitorGameObject->notifyTriggerExit(sensorGameObject);
            }
        }
    }

    static void dispatchContactEventsToComponents(b2WorldId worldId)
    {
        const b2ContactEvents contactEvents = b2World_GetContactEvents(worldId);

        for (int beginIndex = 0; beginIndex < contactEvents.beginCount; ++beginIndex)
        {
            const b2ContactBeginTouchEvent& contactBeginTouchEvent = contactEvents.beginEvents[beginIndex];

            GameObject* firstGameObject =
                static_cast<GameObject*>(b2Shape_GetUserData(contactBeginTouchEvent.shapeIdA));

            GameObject* secondGameObject =
                static_cast<GameObject*>(b2Shape_GetUserData(contactBeginTouchEvent.shapeIdB));

            if (firstGameObject && secondGameObject)
            {
                firstGameObject->notifyCollisionEnter(secondGameObject);
                secondGameObject->notifyCollisionEnter(firstGameObject);
            }
        }

        for (int endIndex = 0; endIndex < contactEvents.endCount; ++endIndex)
        {
            const b2ContactEndTouchEvent& contactEndTouchEvent = contactEvents.endEvents[endIndex];

            if (!b2Shape_IsValid(contactEndTouchEvent.shapeIdA) ||
                !b2Shape_IsValid(contactEndTouchEvent.shapeIdB))
            {
                continue;
            }

            GameObject* firstGameObject =
                static_cast<GameObject*>(b2Shape_GetUserData(contactEndTouchEvent.shapeIdA));

            GameObject* secondGameObject =
                static_cast<GameObject*>(b2Shape_GetUserData(contactEndTouchEvent.shapeIdB));

            if (firstGameObject && secondGameObject)
            {
                firstGameObject->notifyCollisionExit(secondGameObject);
                secondGameObject->notifyCollisionExit(firstGameObject);
            }
        }
    }


    void PhysicsManager::fixedUpdate(float fixedDeltaTime)
    {
        if (!b2World_IsValid(m_worldId))
            return;

        b2World_Step(m_worldId, fixedDeltaTime, m_subStepCount);

        dispatchSensorEventsToComponents(m_worldId);

        dispatchContactEventsToComponents(m_worldId);
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
