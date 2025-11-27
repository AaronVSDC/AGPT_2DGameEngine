#include "BoxColliderComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "RigidBodyComponent.h"
#include "PhysicsManager.h"

namespace Papyrus
{
    BoxColliderComponent::~BoxColliderComponent()
    {
        if (b2Shape_IsValid(m_Shape))
        {
            b2DestroyShape(m_Shape,false);
            m_Shape = {};
        }
    }

    void BoxColliderComponent::start()
    {
        auto* gameObject = getOwner();
        auto* texture = gameObject->getComponent<TextureComponent>();
        if (!texture) return; 

        const b2Vec2 sizePx = texture->getSize();
        m_WidthPixel = sizePx.x;
        m_HeightPixel = sizePx.y;

        // Ensure a rigidbody exists. If you didn't add one, make it static.
        auto* rb = gameObject->getComponent<RigidBodyComponent>();
        if (!rb)
        {
            gameObject->addComponent(std::make_unique<RigidBodyComponent>(BodyType::Static));
            rb = gameObject->getComponent<RigidBodyComponent>();
            rb->start();
        }

        auto& pm = PhysicsManager::getInstance();

        const float halfXMeter = pm.pixelToMeter(m_WidthPixel * 0.5f);
        const float halfYMeter = pm.pixelToMeter(m_HeightPixel * 0.5f);

        b2ShapeDef sd = b2DefaultShapeDef();
        sd.isSensor = m_IsTrigger;
        sd.density = m_Density;
        sd.friction = m_Friction;
        sd.restitution = m_Restitution;

        b2Polygon box = b2MakeBox(halfXMeter, halfYMeter);
        m_Shape = b2CreatePolygonShape(rb->bodyId(), &sd, &box);

        rb->setPositionPixels(gameObject->m_Transform.position.x, gameObject->m_Transform.position.y);
    }
}
