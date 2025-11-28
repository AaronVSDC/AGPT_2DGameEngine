#include "InputManager.h"
#include <iostream>
#include <cmath>

namespace Papyrus
{
    InputManager::InputManager()
    {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD); 
        // Make sure the gamepad subsystem is initialized somewhere in your boot code.
        // If you don't have a centralized init, doing it here is fine.
        if (!SDL_InitSubSystem(SDL_INIT_GAMEPAD))
        {
            std::cerr << "SDL_InitSubSystem(SDL_INIT_GAMEPAD) failed: " << SDL_GetError() << "\n";
        }

        // Keyboard state arrays
        int keyboardCount = 0;
        const bool* keyboardState = SDL_GetKeyboardState(&keyboardCount);
        (void)keyboardState;

        m_currentKeyboardState.resize(keyboardCount, 0);
        m_previousKeyboardState.resize(keyboardCount, 0);
    }

    InputManager::~InputManager() = default;

    bool InputManager::processInput()
    {
        // Pump events so SDL updates input state
        SDL_Event event{};
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                return false;
        }

        updateKeyboardState();

        // Update controllers
        for (auto& controller : m_controllers)
        {
            if (controller && controller->isOpen())
                controller->update();
        }

        // Keyboard bindings
        for (auto& binding : m_keyboardBindings)
        {
            const KeyboardBindingKey& key = binding.first;
            Command* command = binding.second.get();

            bool shouldExecute = false;
            if (key.state == KeyState::Down)    shouldExecute = isKeyboardDown(key.scanCode);
            if (key.state == KeyState::Pressed) shouldExecute = isKeyboardPressed(key.scanCode);
            if (key.state == KeyState::Up)      shouldExecute = isKeyboardUp(key.scanCode);

            if (shouldExecute && command)
                command->execute();
        }

        // Controller button bindings
        for (auto& binding : m_controllerBindings)
        {
            const ControllerBindingKey& key = binding.first;
            Command* command = binding.second.get();

            if (key.controllerSlot < 0 || key.controllerSlot >= static_cast<int>(m_controllers.size()))
                continue;

            Controller* controller = m_controllers[key.controllerSlot].get();
            if (!controller || !controller->isOpen())
                continue;

            bool shouldExecute = false;
            if (key.state == KeyState::Down)    shouldExecute = controller->isDown(key.button);
            if (key.state == KeyState::Pressed) shouldExecute = controller->isPressed(key.button);
            if (key.state == KeyState::Up)      shouldExecute = controller->isUp(key.button);

            if (shouldExecute && command)
                command->execute();
        }

        // Controller stick bindings
        for (auto& stickBinding : m_stickBindings)
        {
            if (stickBinding.controllerSlot < 0 || stickBinding.controllerSlot >= static_cast<int>(m_controllers.size()))
                continue;

            Controller* controller = m_controllers[stickBinding.controllerSlot].get();
            if (!controller || !controller->isOpen())
                continue;

            b2Vec2 value = (stickBinding.stick == Stick::Left)
                ? controller->getLeftThumb()
                : controller->getRightThumb();

            const float magnitude = std::sqrt(value.x * value.x + value.y * value.y);
            if (magnitude < stickBinding.deadzone)
                continue;

            if (stickBinding.normalize && magnitude > 0.0f)
            {
                value.x /= magnitude;
                value.y /= magnitude;
            }

            if (stickBinding.command)
                stickBinding.command->execute(value);
        }

        return true;
    }

    void InputManager::addKeyboardCommand(SDL_Scancode scanCode, KeyState state, std::unique_ptr<Command> command)
    {
        KeyboardBindingKey key{ scanCode, state };
        m_keyboardBindings[key] = std::move(command);
    }

    bool InputManager::addController(int controllerSlot)
    {
        int count = 0;
        SDL_JoystickID* instanceIds = SDL_GetGamepads(&count);
        if (!instanceIds || count <= 0)
        {
            std::cerr << "SDL_GetGamepads found no gamepads: " << SDL_GetError() << "\n";
            if (instanceIds) SDL_free(instanceIds);
            return false;
        }

        if (controllerSlot < 0 || controllerSlot >= count)
        {
            std::cerr << "Controller slot out of range. slot=" << controllerSlot << " count=" << count << "\n";
            SDL_free(instanceIds);
            return false;
        }

        const SDL_JoystickID instanceId = instanceIds[controllerSlot];
        SDL_free(instanceIds);

        // Ensure vector can contain this slot
        if (controllerSlot >= static_cast<int>(m_controllers.size()))
            m_controllers.resize(controllerSlot + 1);

        m_controllers[controllerSlot] = std::make_unique<Controller>(instanceId);

        if (!m_controllers[controllerSlot] || !m_controllers[controllerSlot]->isOpen())
        {
            std::cerr << "Failed to open controller at slot " << controllerSlot << "\n";
            m_controllers[controllerSlot].reset();
            return false;
        }

        return true;
    }

    void InputManager::addControllerCommand(int controllerSlot, ControllerButton button, KeyState state, std::unique_ptr<Command> command)
    {
        ControllerBindingKey key{ controllerSlot, button, state };
        m_controllerBindings[key] = std::move(command);
    }

    void InputManager::addControllerStickCommand(int controllerSlot, Stick stick, float deadzone, bool normalize, std::unique_ptr<AnalogCommand> command)
    {
        StickBinding binding{};
        binding.controllerSlot = controllerSlot;
        binding.stick = stick;
        binding.deadzone = deadzone;
        binding.normalize = normalize;
        binding.command = std::move(command);

        m_stickBindings.emplace_back(std::move(binding));
    }

    void InputManager::updateKeyboardState()
    {
        m_previousKeyboardState = m_currentKeyboardState;

        int keyboardCount = 0;
        const bool* rawKeyboardState = SDL_GetKeyboardState(&keyboardCount);
        if (keyboardCount <= 0 || !rawKeyboardState)
            return;

        if (static_cast<int>(m_currentKeyboardState.size()) != keyboardCount)
        {
            m_currentKeyboardState.assign(keyboardCount, 0);
            m_previousKeyboardState.assign(keyboardCount, 0);
        }

        for (int index = 0; index < keyboardCount; ++index)
        {
            m_currentKeyboardState[index] = rawKeyboardState[index] ? 1 : 0;
        }
    }

    bool InputManager::isKeyboardDown(SDL_Scancode scanCode) const
    {
        const int index = static_cast<int>(scanCode);
        if (index < 0 || index >= static_cast<int>(m_currentKeyboardState.size()))
            return false;
        return m_currentKeyboardState[index] != 0;
    }

    bool InputManager::isKeyboardPressed(SDL_Scancode scanCode) const
    {
        const int index = static_cast<int>(scanCode);
        if (index < 0 || index >= static_cast<int>(m_currentKeyboardState.size()))
            return false;

        const bool current = m_currentKeyboardState[index] != 0; 
        const bool previous = m_previousKeyboardState[index] != 0;
        return current && !previous;
    }

    bool InputManager::isKeyboardUp(SDL_Scancode scanCode) const
    {
        const int index = static_cast<int>(scanCode);
        if (index < 0 || index >= static_cast<int>(m_currentKeyboardState.size()))
            return false;

        const bool current = m_currentKeyboardState[index] != 0;
        const bool previous = m_previousKeyboardState[index] != 0;
        return !current && previous;
    }
}
