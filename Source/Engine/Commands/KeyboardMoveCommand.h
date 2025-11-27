#ifndef MOVE_UP_COMMAND_H
#define MOVE_UP_COMMAND_H

#include "Command.h"
#include "GameObject.h"
#include "MoveComponent.h"

namespace Papyrus
{
    class MoveUpCommand final : public Command 
    {
    public:
        explicit MoveUpCommand(GameObject* gameObject)  
            : m_gameObject(gameObject) {} 

        void execute() override 
        { 
            if (!m_gameObject) return;

            auto* moveComponent = m_gameObject->getComponent<MoveComponent>();
            if (!moveComponent) return;

            moveComponent->addAcceleration({ 0.0f, -1.0f });
        }

    private:
        GameObject* m_gameObject{};
    };


    class MoveDownCommand final : public Command
    {
    public:
        explicit MoveDownCommand(GameObject* gameObject)
            : m_gameObject(gameObject) {}

        void execute() override
        {
            if (!m_gameObject) return;

            auto* moveComponent = m_gameObject->getComponent<MoveComponent>();
            if (!moveComponent) return;

            moveComponent->addAcceleration({ 0.0f, 1.0f });
        }

    private:
        GameObject* m_gameObject{};
    };

    class MoveLeftCommand final : public Command 
    {
    public:
        explicit MoveLeftCommand(GameObject* gameObject)
            : m_gameObject(gameObject) {}

        void execute() override
        {
            if (!m_gameObject) return;

            auto* moveComponent = m_gameObject->getComponent<MoveComponent>();
            if (!moveComponent) return;

            moveComponent->addAcceleration({ -1.0f, 0.0f });
        }

    private:
        GameObject* m_gameObject{};
    };

    class MoveRightCommand final : public Command
    { 
    public:
        explicit MoveRightCommand(GameObject* gameObject)
            : m_gameObject(gameObject) {}

        void execute() override
        {
            if (!m_gameObject) return;

            auto* moveComponent = m_gameObject->getComponent<MoveComponent>();
            if (!moveComponent) return;

            moveComponent->addAcceleration({ 1.0f, 0.0f });
        }

    private:
        GameObject* m_gameObject{};
    };
}

#endif
