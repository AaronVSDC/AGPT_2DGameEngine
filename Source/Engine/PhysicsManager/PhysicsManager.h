#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include <box2d/box2d.h>
#include <vector>
#include <utility>

#include "Singleton.h"

namespace Papyrus
{
    class GameObject;

    struct OverlapBeginEvent
    {
        GameObject* firstGameObject = nullptr;
        GameObject* secondGameObject = nullptr;
    };

    class PhysicsManager final : public Singleton<PhysicsManager>
    {
    public:
        PhysicsManager();
        ~PhysicsManager();

        void fixedUpdate(float fixedDeltaTime);

        b2WorldId getWorld() const { return m_worldId; }

        // Units helpers (your engine uses pixels; Box2D uses meters)
        float getPixelsPerMeter() const { return m_pixelsPerMeter; }
        float pixelsToMeters(float pixels) const { return pixels / m_pixelsPerMeter; }
        float metersToPixels(float meters) const { return meters * m_pixelsPerMeter; }
        b2Vec2 pixelsToMeters(b2Vec2 pixels) const { return { pixels.x / m_pixelsPerMeter, pixels.y / m_pixelsPerMeter }; }
        b2Vec2 metersToPixels(b2Vec2 meters) const { return { meters.x * m_pixelsPerMeter, meters.y * m_pixelsPerMeter }; }

        // Poll overlaps that began during the last physics step
        std::vector<OverlapBeginEvent> consumeOverlapBeginEvents();

    private:
        void collectSensorOverlapBeginEvents();

        b2WorldId m_worldId = {};
        int m_subStepCount = 4;

        float m_pixelsPerMeter = 100.0f;

        std::vector<OverlapBeginEvent> m_overlapBeginEventsThisStep;
    };
}

#endif
