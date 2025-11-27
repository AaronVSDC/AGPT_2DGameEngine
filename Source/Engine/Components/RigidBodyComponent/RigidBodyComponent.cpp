#include "RigidBodyComponent.h"
#include "PhysicsManager.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "BoxColliderComponent.h"

namespace Papyrus
{
    static b2Vec2 GetVisualSizePx(GameObject* go)
    {
        if (!go) return { 0, 0 };
        if (auto* col = go->getComponent<BoxColliderComponent>()) return col->getSizePx();
        if (auto* tex = go->getComponent<TextureComponent>()) return tex->getSize();
        return { 0, 0 };
    }

    b2BodyType RigidBodyComponent::toB2BodyType(BodyType t) const 
    {
        switch (t) 
        {
        case BodyType::Static:    return b2_staticBody;
        case BodyType::Kinematic: return b2_kinematicBody;
        default:                  return b2_dynamicBody;
        }
    }

    RigidBodyComponent::~RigidBodyComponent()
    {
        if (b2Body_IsValid(m_body))
        {
            b2DestroyBody(m_body);
            m_body = {};
        }
    }

    void RigidBodyComponent::start()
    {
        auto& pm = PhysicsManager::getInstance();

        GameObject* go = getOwner();
        const b2Vec2 sizePx = GetVisualSizePx(go);
        const b2Vec2 topLeftPx = go->m_Transform.position;
        const b2Vec2 centerPx{ topLeftPx.x + sizePx.x * 0.5f, topLeftPx.y + sizePx.y * 0.5f };

        b2BodyDef bd = b2DefaultBodyDef();
        bd.type = toB2(m_type);
        bd.position = pm.pxToM(centerPx);
        bd.rotation = b2MakeRot(go->m_Transform.rotation);
        bd.fixedRotation = m_fixedRotation;

        // This makes BodyMoveEvents carry the GameObject*
        bd.userData = go;

        m_body = b2CreateBody(pm.getWorld(), &bd);
    }

    void RigidBodyComponent::fixedUpdate(float)
    {
        if (!b2Body_IsValid(m_body)) return;

        // If you "manipulate positions" directly, do it on a kinematic body
        if (m_type == BodyType::Kinematic)
        {
            auto& pm = PhysicsManager::getInstance();
            GameObject* go = getOwner();

            const b2Vec2 sizePx = GetVisualSizePx(go);
            const b2Vec2 topLeftPx = go->m_Transform.position;
            const b2Vec2 centerPx{ topLeftPx.x + sizePx.x * 0.5f, topLeftPx.y + sizePx.y * 0.5f };

            b2Body_SetTransform(m_body, pm.pxToM(centerPx), b2MakeRot(go->m_Transform.rotation));
        }
    }

    void RigidBodyComponent::setLinearVelocity(float vx_mps, float vy_mps)
    {
        if (!b2Body_IsValid(m_body)) return;
        b2Body_SetLinearVelocity(m_body, b2Vec2{ vx_mps, vy_mps });
    }

    void RigidBodyComponent::setPositionPixels(float xPx, float yPx)
    {
        if (!b2Body_IsValid(m_body)) return;

        auto& pm = PhysicsManager::getInstance();
        GameObject* go = getOwner();

        const b2Vec2 sizePx = GetVisualSizePx(go);
        const b2Vec2 centerPx{ xPx + sizePx.x * 0.5f, yPx + sizePx.y * 0.5f };

        b2Body_SetTransform(m_body, pm.pxToM(centerPx), b2Body_GetRotation(m_body));
    }
}
