#include "InputManager.h"
#include <box2d\math_functions.h>

#include <cassert>
#include <iostream>
#include <algorithm>

namespace Papyrus
{

    bool InputManager::processInput()
    {
        for (auto& controller : m_Controllers)
        {
            if (controller)
                controller->update();
        }

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
                return false;

            if (e.type == SDL_EVENT_KEY_DOWN && !e.key.repeat)
            {
                for (auto& binding : m_KeyboardCommands)
                {
                    if (binding.state == KeyState::Pressed &&
                        binding.key == e.key.scancode)
                    {
                        binding.command->execute();
                    }
                }
            }

            if (e.type == SDL_EVENT_KEY_UP)
            {
                for (auto& binding : m_KeyboardCommands)
                {
                    if (binding.state == KeyState::Up &&
                        binding.key == e.key.scancode)
                    {
                        binding.command->execute();
                    }
                }
            }
        }

        const bool* keyboardState = SDL_GetKeyboardState(nullptr);

        for (auto& binding : m_KeyboardCommands)
        {
            if (binding.state == KeyState::Down &&
                keyboardState[binding.key])
            {
                binding.command->execute();
            }
        }

        for (auto& binding : m_ControllerCommands)
        {
            Controller* controller = m_Controllers[binding.controllerIdx].get();
            if (!controller)
                continue;

            switch (binding.state)
            {
            case KeyState::Pressed:
                if (controller->isPressed(binding.button))
                    binding.command->execute();
                break;

            case KeyState::Down:
                if (controller->isDown(binding.button))
                    binding.command->execute();
                break;

            case KeyState::Up:
                if (controller->isUp(binding.button))
                    binding.command->execute();
                break;
            }
        }

        for (auto& binding : m_StickCommands)
        {
            Controller* controller = m_Controllers[binding.controllerIdx].get();
            if (!controller)
                continue;

            b2Vec2 raw = (binding.stick == Stick::Left) 
                ? controller->getLeftThumb()
                : controller->getRightThumb();

            const float mag = std::sqrt(raw.x * raw.x + raw.y * raw.y); 

            if (mag <= binding.deadzone)
            {
                raw = b2Vec2_zero; 
            }
            else if (binding.normalize && mag > 0.f)
            {
                const float t = (mag - binding.deadzone) / (1.f - binding.deadzone);
                const float clampedT = std::clamp(t, 0.f, 1.f);

                raw.x = (raw.x / mag) * clampedT;
                raw.y = (raw.y / mag) * clampedT;
            }

            if (raw.x != 0.f || raw.y != 0.f)
            {
                binding.command->execute(raw);
            }
        }

        return true;
    }


    void InputManager::addController(uint8_t index)
    {
        m_Controllers.push_back(std::make_unique<Controller>(index));
    }

    void InputManager::addControllerCommand(uint8_t controllerIdx, ControllerButton button, KeyState state, std::unique_ptr<Command> command)
    {
        assert(controllerIdx < m_Controllers.size() && "Trying to access controller index that does not exist, create the right controller first or check if indexing properly");
        m_ControllerCommands.push_back(ControllerBinding{ controllerIdx, button, state, std::move(command) });
    }
    void InputManager::addControllerStickCommand(uint8_t controllerIdx, Stick stick, float deadzone, bool normalize, std::unique_ptr<AnalogCommand> command)
    {
        assert(controllerIdx < m_Controllers.size());
        m_StickCommands.push_back(StickBinding{ controllerIdx, stick, deadzone, normalize, std::move(command) });
    }

    void InputManager::addKeyboardCommand(SDL_Scancode key, KeyState state, std::unique_ptr<Command> command)
    {
        m_KeyboardCommands.push_back(KeyboardBinding{ key, state, std::move(command) });
    }
}