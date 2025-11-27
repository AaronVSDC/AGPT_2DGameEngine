#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H
#include <mutex>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Singleton.h"     

namespace Papyrus
{
    class PhysicsManager final : public Singleton<PhysicsManager>
    {
    public:
         
        void fixedUpdate(float fixedDeltaTime); 

    private:
    };
}
#endif