#include "BoxColliderComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "PhysicsBodyComponent.h"
#include "PhysicsManager.h"

namespace Papyrus
{
    BoxColliderComponent::~BoxColliderComponent()
    {
        if (b2Shape_IsValid(m_shapeId))
        {
            b2DestroyShape(m_shapeId, true); 
            m_shapeId = {};
        }
    }

    void BoxColliderComponent::start()
    {
        GameObject* owner = getOwner();

        auto* textureComponent = owner->getComponent<TextureComponent>();
        auto* physicsBodyComponent = owner->getComponent<PhysicsBodyComponent>();
        if (!textureComponent || !physicsBodyComponent)
            return;

        const b2Vec2 sizePx = textureComponent->getSize();
        m_widthPixels = sizePx.x;
        m_heightPixels = sizePx.y;

        PhysicsManager& physicsManager = PhysicsManager::getInstance();

        const float halfWidthMeters = physicsManager.pixelsToMeters(m_widthPixels * 0.5f);
        const float halfHeightMeters = physicsManager.pixelsToMeters(m_heightPixels * 0.5f);

        b2Polygon boxPolygon = b2MakeBox(halfWidthMeters, halfHeightMeters);

        b2ShapeDef shapeDefinition = b2DefaultShapeDef();
        shapeDefinition.isSensor = true;                // overlap only
        shapeDefinition.enableSensorEvents = true;      // REQUIRED
        shapeDefinition.userData = owner;               // map events to GameObject

        m_shapeId = b2CreatePolygonShape(
            physicsBodyComponent->getBodyId(),
            &shapeDefinition,
            &boxPolygon
        );
    }
}
