#ifndef COMMAND_H
#define COMMAND_H
#include <box2d\math_functions.h>

namespace Papyrus
{ 
    class Command
    {
    public:
        virtual ~Command() = default;
        virtual void execute() = 0;
    };
    class AnalogCommand
    {
    public:
        virtual ~AnalogCommand() = default;
        virtual void execute(const b2Vec2& value) = 0;
    };
}

#endif