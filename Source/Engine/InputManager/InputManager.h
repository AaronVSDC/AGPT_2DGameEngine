#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <memory>
#include <unordered_map>
#include <vector>
#include <SDL3/SDL.h>
#include <box2d/math_functions.h>

#include "Singleton.h"
#include "Controller.h"
#include "Command.h"
#include "PapyrusAPI.h"

namespace Papyrus
{
    class AnalogCommand; // you already have this

    class PAPYRUS_API InputManager final : public Singleton<InputManager> 
    {
    public:
        InputManager();
        ~InputManager();

        bool processInput(); // returns false on quit

        // Keyboard
        void addKeyboardCommand(SDL_Scancode scanCode, KeyState state, std::unique_ptr<Command> command);

        // Controller
        bool addController(int controllerSlot); // 0 = first connected controller
        void addControllerCommand(int controllerSlot, ControllerButton button, KeyState state, std::unique_ptr<Command> command);
        void addControllerStickCommand(int controllerSlot, Stick stick, float deadzone, bool normalize, std::unique_ptr<AnalogCommand> command);

    private:
        struct KeyboardBindingKey
        {
            SDL_Scancode scanCode{};
            KeyState state{};
            bool operator==(const KeyboardBindingKey& other) const
            {
                return scanCode == other.scanCode && state == other.state;
            }
        };

        struct KeyboardBindingKeyHash
        {
            size_t operator()(const KeyboardBindingKey& key) const
            {
                return (static_cast<size_t>(key.scanCode) << 2) ^ static_cast<size_t>(key.state);
            }
        };

        struct ControllerBindingKey
        {
            int controllerSlot{};
            ControllerButton button{};
            KeyState state{};
            bool operator==(const ControllerBindingKey& other) const
            {
                return controllerSlot == other.controllerSlot && button == other.button && state == other.state;
            }
        };

        struct ControllerBindingKeyHash
        {
            size_t operator()(const ControllerBindingKey& key) const
            {
                return (static_cast<size_t>(key.controllerSlot) << 24)
                    ^ (static_cast<size_t>(static_cast<int>(key.button)) << 8)
                    ^ static_cast<size_t>(key.state);
            }
        };

        struct StickBinding
        {
            int controllerSlot{};
            Stick stick{};
            float deadzone{};
            bool normalize{};
            std::unique_ptr<AnalogCommand> command{};
        };

    private:
        void updateKeyboardState();

        bool isKeyboardDown(SDL_Scancode scanCode) const;
        bool isKeyboardPressed(SDL_Scancode scanCode) const;
        bool isKeyboardUp(SDL_Scancode scanCode) const;

    private:
        std::vector<std::unique_ptr<Controller>> m_controllers;

        std::unordered_map<KeyboardBindingKey, std::unique_ptr<Command>, KeyboardBindingKeyHash> m_keyboardBindings;
        std::unordered_map<ControllerBindingKey, std::unique_ptr<Command>, ControllerBindingKeyHash> m_controllerBindings;
        std::vector<StickBinding> m_stickBindings;

        std::vector<uint8_t> m_currentKeyboardState;
        std::vector<uint8_t> m_previousKeyboardState;
    };
}

#endif
