#ifndef COLLISION_SIZE_H
#define COLLISION_SIZE_H

#include <box2d/math_functions.h>

namespace Papyrus
{
    class GameObject;

    b2Vec2 getCollisionSizePixels(const GameObject* owner);
}

#endif
