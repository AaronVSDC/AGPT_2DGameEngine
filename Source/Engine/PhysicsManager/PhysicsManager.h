#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include <box2d\box2d.h>

#include "Singleton.h"

namespace Papyrus
{
    class PhysicsManager final : public Singleton<PhysicsManager>
    {
    public: 
        PhysicsManager();

        void fixedUpdate(float fixedDeltaTime);
        b2World& getWorld() { return m_World;  }
    private:
        b2World m_World;
    };
}

#endif