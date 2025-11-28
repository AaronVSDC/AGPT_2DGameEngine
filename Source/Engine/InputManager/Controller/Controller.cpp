#include "Controller.h"
#include <iostream>
#include <cmath>
#include <algorithm>

namespace Papyrus
{
    class Controller::ControllerImpl
    {
    public:
        explicit ControllerImpl(SDL_JoystickID instanceId)
            : m_instanceId(instanceId)
        {
            m_gamepad = SDL_OpenGamepad(instanceId);
            if (!m_gamepad)
            {
                std::cerr << "SDL_OpenGamepad failed (instanceId=" << instanceId
                    << "): " << SDL_GetError() << "\n";
            }
        }

        ~ControllerImpl()
        {
            if (m_gamepad)
            {
                SDL_CloseGamepad(m_gamepad);
                m_gamepad = nullptr;
            }
        }

        void update()
        {
            if (!m_gamepad)
                return;

            m_previousButtons = m_currentButtons;
            m_currentButtons = 0;

            for (int buttonIndex = 0; buttonIndex < SDL_GAMEPAD_BUTTON_COUNT; ++buttonIndex)
            {
                if (SDL_GetGamepadButton(m_gamepad, static_cast<SDL_GamepadButton>(buttonIndex)))
                {
                    m_currentButtons |= (1u << buttonIndex);
                }
            }
        }

        bool isDown(ControllerButton button) const
        {
            const uint32_t mask = 1u << static_cast<int>(button);
            return (m_currentButtons & mask) != 0;
        }

        bool isPressed(ControllerButton button) const
        {
            const uint32_t mask = 1u << static_cast<int>(button);
            return (m_currentButtons & mask) && !(m_previousButtons & mask);
        }

        bool isUp(ControllerButton button) const
        {
            const uint32_t mask = 1u << static_cast<int>(button);
            return !(m_currentButtons & mask) && (m_previousButtons & mask);
        }

        b2Vec2 getLeftThumb() const
        {
            return getStick(SDL_GAMEPAD_AXIS_LEFTX, SDL_GAMEPAD_AXIS_LEFTY);
        }

        b2Vec2 getRightThumb() const
        {
            return getStick(SDL_GAMEPAD_AXIS_RIGHTX, SDL_GAMEPAD_AXIS_RIGHTY);
        }

        bool isOpen() const { return m_gamepad != nullptr; }
        SDL_JoystickID getInstanceId() const { return m_instanceId; }

    private:
        static float normalizeAxisValue(int value)
        {
            // SDL axis is typically -32768..32767
            if (value >= 0)
                return static_cast<float>(value) / 32767.0f;

            return static_cast<float>(value) / 32768.0f;
        }

        static void applyDeadzone(float& x, float& y)
        {
            constexpr float deadZone = 0.15f;
            if (std::fabs(x) < deadZone) x = 0.0f;
            if (std::fabs(y) < deadZone) y = 0.0f;
        }

        b2Vec2 getStick(SDL_GamepadAxis axisX, SDL_GamepadAxis axisY) const
        {
            if (!m_gamepad)
                return { 0.0f, 0.0f };

            const int rawX = SDL_GetGamepadAxis(m_gamepad, axisX);
            const int rawY = SDL_GetGamepadAxis(m_gamepad, axisY);

            float x = normalizeAxisValue(rawX);
            float y = normalizeAxisValue(rawY);

            applyDeadzone(x, y);

            // Invert Y so up is positive (match your existing convention)
            return { x, -y };
        }

    private:
        SDL_JoystickID m_instanceId{};
        SDL_Gamepad* m_gamepad{};

        uint32_t m_currentButtons{};
        uint32_t m_previousButtons{};
    };

    Controller::Controller(SDL_JoystickID instanceId)
        : m_implementation(std::make_unique<ControllerImpl>(instanceId))
    {
    }

    Controller::~Controller() = default;

    void Controller::update() { m_implementation->update(); }

    bool Controller::isDown(ControllerButton button) const { return m_implementation->isDown(button); }
    bool Controller::isPressed(ControllerButton button) const { return m_implementation->isPressed(button); }
    bool Controller::isUp(ControllerButton button) const { return m_implementation->isUp(button); }

    b2Vec2 Controller::getLeftThumb() const { return m_implementation->getLeftThumb(); }
    b2Vec2 Controller::getRightThumb() const { return m_implementation->getRightThumb(); }

    bool Controller::isOpen() const { return m_implementation->isOpen(); }
    SDL_JoystickID Controller::getInstanceId() const { return m_implementation->getInstanceId(); }
}
