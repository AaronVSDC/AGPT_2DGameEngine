#ifndef EXPLOSION_UTILITY_H
#define EXPLOSION_UTILITY_H

#include <string>

namespace Papyrus
{
    class GameObject;

    void explodeAndDie(
        GameObject* target,
        const std::string& explosionTexturePath,
        int columns,
        int rows,
        int frames,
        float framesPerSecond
    );
}

#endif
