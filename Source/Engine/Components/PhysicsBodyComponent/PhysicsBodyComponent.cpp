#include "PhysicsBodyComponent.h"
#include "GameObject.h"
#include "PhysicsManager.h"
#include "TextureComponent.h"
#include "CollisionSize.h" 

namespace Papyrus
{
    PhysicsBodyComponent::PhysicsBodyComponent(PhysicsBodyType bodyType)
        : m_bodyType(bodyType)
    {
    }

    PhysicsBodyComponent::~PhysicsBodyComponent()
    {
        if (b2Body_IsValid(m_bodyId))
        {
            b2DestroyBody(m_bodyId);
            m_bodyId = {};
        }
    }

    void PhysicsBodyComponent::start()
    {
        PhysicsManager& physicsManager = PhysicsManager::getInstance();
        GameObject* owner = getOwner();

        const b2Vec2 sizePx = getCollisionSizePixels(owner);


        b2Vec2 centerPx{
            owner->m_Transform.position.x + sizePx.x * 0.5f,
            owner->m_Transform.position.y + sizePx.y * 0.5f
        };

        b2BodyDef bodyDefinition = b2DefaultBodyDef();
        bodyDefinition.type =
            (m_bodyType == PhysicsBodyType::Static)
            ? b2_staticBody
            : b2_kinematicBody;

        bodyDefinition.position = physicsManager.pixelsToMeters(centerPx);
        bodyDefinition.rotation = b2MakeRot(owner->m_Transform.rotation);

        m_bodyId = b2CreateBody(physicsManager.getWorld(), &bodyDefinition);
    }

    void PhysicsBodyComponent::fixedUpdate(float)
    {
        if (m_bodyType != PhysicsBodyType::Kinematic)
            return;

        if (!b2Body_IsValid(m_bodyId)) 
            return;

        PhysicsManager& physicsManager = PhysicsManager::getInstance();
        GameObject* owner = getOwner();

        const b2Vec2 sizePx = getCollisionSizePixels(owner); 


        b2Vec2 centerPx{
            owner->m_Transform.position.x + sizePx.x * 0.5f,
            owner->m_Transform.position.y + sizePx.y * 0.5f
        }; 

        b2Body_SetTransform(
            m_bodyId,
            physicsManager.pixelsToMeters(centerPx),
            b2MakeRot(owner->m_Transform.rotation)
        );
    }
}
