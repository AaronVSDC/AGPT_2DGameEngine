#ifndef EXPLOSION_UTILITY_H
#define EXPLOSION_UTILITY_H

#include <string>

namespace Papyrus
{
    class GameObject;
}


namespace xc
{
    void explodeAndDie(
        Papyrus::GameObject* target,  
        const std::string& explosionTexturePath,
        int columns,
        int rows,
        int frames,
        float framesPerSecond
    );
}

#endif
