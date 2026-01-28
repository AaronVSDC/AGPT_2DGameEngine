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

    static inline GameObject* getGOFromShapeUserData(b2ShapeId shapeId)
    {
        if (!b2Shape_IsValid(shapeId))
            return nullptr;
        return static_cast<GameObject*>(b2Shape_GetUserData(shapeId));
    }

    static inline bool alive(GameObject* go)
    {
        return go && !go->isPendingRemoval();
    }

    static void dispatchSensorEventsToComponents(b2WorldId worldId)
    {
        const b2SensorEvents sensorEvents = b2World_GetSensorEvents(worldId);

        // -----------------------------
        // Begin touch (SENSOR)
        // -----------------------------
        for (int i = 0; i < sensorEvents.beginCount; ++i)
        {
            const b2SensorBeginTouchEvent& e = sensorEvents.beginEvents[i];

            // FIX: validity checks on BEGIN events too
            if (!b2Shape_IsValid(e.sensorShapeId) || !b2Shape_IsValid(e.visitorShapeId))
                continue;

            GameObject* sensorGO  = getGOFromShapeUserData(e.sensorShapeId);
            GameObject* visitorGO = getGOFromShapeUserData(e.visitorShapeId);

            if (!alive(sensorGO) || !alive(visitorGO))
                continue;

            // First notify
            sensorGO->notifyTriggerEnter(visitorGO);

            // FIX: re-check because callbacks may mark objects for removal
            if (alive(sensorGO) && alive(visitorGO))
            {
                visitorGO->notifyTriggerEnter(sensorGO);
            }
        }

        // -----------------------------
        // End touch (SENSOR)
        // -----------------------------
        for (int i = 0; i < sensorEvents.endCount; ++i)
        {
            const b2SensorEndTouchEvent& e = sensorEvents.endEvents[i];

            if (!b2Shape_IsValid(e.sensorShapeId) || !b2Shape_IsValid(e.visitorShapeId))
                continue;

            GameObject* sensorGO  = getGOFromShapeUserData(e.sensorShapeId);
            GameObject* visitorGO = getGOFromShapeUserData(e.visitorShapeId);

            if (!alive(sensorGO) || !alive(visitorGO))
                continue;

            sensorGO->notifyTriggerExit(visitorGO);

            // FIX: re-check after first notify
            if (alive(sensorGO) && alive(visitorGO))
            {
                visitorGO->notifyTriggerExit(sensorGO);
            }
        }
    }

    static void dispatchContactEventsToComponents(b2WorldId worldId)
    {
        const b2ContactEvents contactEvents = b2World_GetContactEvents(worldId);

        // -----------------------------
        // Begin touch (CONTACT)
        // -----------------------------
        for (int i = 0; i < contactEvents.beginCount; ++i)
        {
            const b2ContactBeginTouchEvent& e = contactEvents.beginEvents[i];

            // FIX: validity checks on BEGIN events too
            if (!b2Shape_IsValid(e.shapeIdA) || !b2Shape_IsValid(e.shapeIdB))
                continue;

            GameObject* a = getGOFromShapeUserData(e.shapeIdA);
            GameObject* b = getGOFromShapeUserData(e.shapeIdB);

            if (!alive(a) || !alive(b))
                continue;

            a->notifyCollisionEnter(b);

            // FIX: re-check after first notify
            if (alive(a) && alive(b))
            {
                b->notifyCollisionEnter(a);
            }
        }

        // -----------------------------
        // End touch (CONTACT)
        // -----------------------------
        for (int i = 0; i < contactEvents.endCount; ++i)
        {
            const b2ContactEndTouchEvent& e = contactEvents.endEvents[i];

            if (!b2Shape_IsValid(e.shapeIdA) || !b2Shape_IsValid(e.shapeIdB))
                continue;

            GameObject* a = getGOFromShapeUserData(e.shapeIdA);
            GameObject* b = getGOFromShapeUserData(e.shapeIdB);

            if (!alive(a) || !alive(b))
                continue;

            a->notifyCollisionExit(b);

            // FIX: re-check after first notify
            if (alive(a) && alive(b))
            {
                b->notifyCollisionExit(a);
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

        for (int i = 0; i < sensorEvents.beginCount; ++i)
        {
            const b2SensorBeginTouchEvent& e = sensorEvents.beginEvents[i];

            // FIX: validity checks here too
            if (!b2Shape_IsValid(e.sensorShapeId) || !b2Shape_IsValid(e.visitorShapeId))
                continue;

            GameObject* first  = getGOFromShapeUserData(e.sensorShapeId);
            GameObject* second = getGOFromShapeUserData(e.visitorShapeId);

            // FIX: skip dying objects; otherwise you store stale pointers
            if (!alive(first) || !alive(second))
                continue;

            OverlapBeginEvent overlapBeginEvent{};
            overlapBeginEvent.firstGameObject = first;
            overlapBeginEvent.secondGameObject = second;

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
