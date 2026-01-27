#ifndef PHYSICS_BODY_COMPONENT_H
#define PHYSICS_BODY_COMPONENT_H

#include <box2d/box2d.h>
#include "BaseComponent.h"
#include "PapyrusAPI.h"

namespace Papyrus
{
    enum class PhysicsBodyType
    {
        Static,
        Kinematic   // follows Transform  
    };

    class PAPYRUS_API PhysicsBodyComponent final : public BaseComponent 
    {
    public:
        explicit PhysicsBodyComponent(PhysicsBodyType bodyType = PhysicsBodyType::Kinematic);

        ~PhysicsBodyComponent() override; 

        void start() override;
        void fixedUpdate(float fixedDeltaTime) override;

        b2BodyId getBodyId() const { return m_bodyId; }

    private:
        PhysicsBodyType m_bodyType;
        b2BodyId m_bodyId{};
    };
}

#endif
