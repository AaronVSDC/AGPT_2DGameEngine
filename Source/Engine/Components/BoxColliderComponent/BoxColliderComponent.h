#pragma once
#include <box2d/box2d.h>
#include "BaseComponent.h"

namespace Papyrus
{
    class BoxColliderComponent final : public BaseComponent
    {
    public: 
        explicit BoxColliderComponent(bool isTrigger = true) : m_isTrigger(isTrigger) {}
        ~BoxColliderComponent() override;

        void start() override;

        b2Vec2 getSizePixels() const { return { m_widthPixels, m_heightPixels }; }
        void setSensor(bool isSensor) { m_isSensor = isSensor; }
        void rebuild(); 


    private:
        bool m_isTrigger = true;

        float m_widthPixels = 0.0f;
        float m_heightPixels = 0.0f;

        b2ShapeId m_shapeId = {};

        b2BodyId  m_bodyId{};
        bool m_isSensor = false;
        b2Vec2 m_size{};
    };

}
