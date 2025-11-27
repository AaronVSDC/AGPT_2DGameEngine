#pragma once
#include <box2d/box2d.h>
#include "BaseComponent.h"

namespace Papyrus
{
    class BoxColliderComponent final : public BaseComponent 
    {
    public:
        explicit BoxColliderComponent(bool isTrigger = false)
            : m_IsTrigger(isTrigger) {}

        ~BoxColliderComponent() override;

        void start() override;

        b2Vec2 getSizePx() const { return { m_WidthPixel, m_HeightPixel }; }

        void setFriction(float f) { m_Friction = f; }
        void setRestitution(float r) { m_Restitution = r; }
        void setDensity(float d) { m_Density = d; }

    private:
        bool m_IsTrigger = false;

        float m_WidthPixel = 0.0f;
        float m_HeightPixel = 0.0f;

        float m_Density = 1.0f;
        float m_Friction = 0.3f;
        float m_Restitution = 0.0f;

        b2ShapeId m_Shape{};
    };
}
