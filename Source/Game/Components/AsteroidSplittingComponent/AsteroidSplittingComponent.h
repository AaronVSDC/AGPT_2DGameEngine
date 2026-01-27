#ifndef ASTEROID_SPLITTING_COMPONENT_H
#define ASTEROID_SPLITTING_COMPONENT_H

#include "BaseComponent.h"
#include <string>

namespace xc
{
    enum class AsteroidSize
    {
        Large,
        Medium,
        Small
    };
    class AsteroidSplittingComponent : public Papyrus::BaseComponent
    {
    public:
        AsteroidSplittingComponent(
            AsteroidSize size,
            const std::string& nextTexturePath
        );

        void split();

    private:
        AsteroidSize m_size;
        std::string m_nextTexturePath;
    };
}

#endif