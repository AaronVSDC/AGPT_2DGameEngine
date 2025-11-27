#include "Controller.h"
#include <cmath>

namespace Papyrus
{
    class Controller::ControllerImpl
    {
    public:
        explicit ControllerImpl(uint8_t index)
            : m_Index(index)
        {
            m_Gamepad = SDL_OpenGamepad(index);
        }

        ~ControllerImpl()
        {
            if (m_Gamepad)
                SDL_CloseGamepad(m_Gamepad);
        }

        void update()
        {
            if (!m_Gamepad)
                return;

            m_PreviousButtons = m_CurrentButtons;
            m_CurrentButtons = 0;

            // Poll all buttons
            for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; ++i) 
            {
                if (SDL_GetGamepadButton(m_Gamepad, static_cast<SDL_GamepadButton>(i)))
                {
                    m_CurrentButtons |= (1u << i);
                }
            }
        }

        bool isDown(ControllerButton button) const
        {
            return (m_CurrentButtons & (1u << static_cast<uint32_t>(button))) != 0;
        }

        bool isPressed(ControllerButton button) const
        {
            const uint32_t mask = 1u << static_cast<uint32_t>(button);
            return (m_CurrentButtons & mask) && !(m_PreviousButtons & mask);
        }

        bool isUp(ControllerButton button) const
        {
            const uint32_t mask = 1u << static_cast<uint32_t>(button);
            return !(m_CurrentButtons & mask) && (m_PreviousButtons & mask);
        }

        b2Vec2 getLeftThumb() const
        {
            if (!m_Gamepad)
                return { 0.0f, 0.0f };

            float x = SDL_GetGamepadAxis(m_Gamepad, SDL_GAMEPAD_AXIS_LEFTX) / 32767.0f; //the division is to normalize it to -1,1
            float y = SDL_GetGamepadAxis(m_Gamepad, SDL_GAMEPAD_AXIS_LEFTY) / 32767.0f;

            applyDeadzone(x, y);
            return { x, -y };
        }

        b2Vec2 getRightThumb() const
        {
            if (!m_Gamepad)
                return { 0.0f, 0.0f };

            float x = SDL_GetGamepadAxis(m_Gamepad, SDL_GAMEPAD_AXIS_RIGHTX) / 32767.0f;
            float y = SDL_GetGamepadAxis(m_Gamepad, SDL_GAMEPAD_AXIS_RIGHTY) / 32767.0f;

            applyDeadzone(x, y);
            return { x, -y };
        }

    private:
        static void applyDeadzone(float& x, float& y)
        {
            constexpr float deadZone = 0.15f;

            if (std::fabs(x) < deadZone) x = 0.0f;
            if (std::fabs(y) < deadZone) y = 0.0f;
        }

    private:
        uint8_t m_Index{};
        SDL_Gamepad* m_Gamepad{};

        uint32_t m_CurrentButtons{};
        uint32_t m_PreviousButtons{};
    };


    Controller::Controller(uint8_t controllerIndex)
        : m_pImpl(std::make_unique<ControllerImpl>(controllerIndex))
    {
    }

    Controller::~Controller() = default;

    bool Controller::isDown(ControllerButton button) const
    {
        return m_pImpl->isDown(button);
    }

    bool Controller::isPressed(ControllerButton button) const
    {
        return m_pImpl->isPressed(button);
    }

    bool Controller::isUp(ControllerButton button) const
    {
        return m_pImpl->isUp(button);
    }

    void Controller::update()
    {
        m_pImpl->update();
    }

    b2Vec2 Controller::getLeftThumb() const
    {
        return m_pImpl->getLeftThumb();
    }

    b2Vec2 Controller::getRightThumb() const
    {
        return m_pImpl->getRightThumb();
    }
}
