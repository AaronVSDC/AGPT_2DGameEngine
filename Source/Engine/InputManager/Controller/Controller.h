#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>
#include <cstdint>
#include <box2d/math_functions.h>
#include <SDL3\SDL.h> 


namespace Papyrus
{
    enum class ControllerButton : uint32_t
    {
        DPadUp = SDL_GAMEPAD_BUTTON_DPAD_UP,
        DPadDown = SDL_GAMEPAD_BUTTON_DPAD_DOWN,
        DPadLeft = SDL_GAMEPAD_BUTTON_DPAD_LEFT,
        DPadRight = SDL_GAMEPAD_BUTTON_DPAD_RIGHT,

        Start = SDL_GAMEPAD_BUTTON_START,
        Back = SDL_GAMEPAD_BUTTON_BACK,

        LeftThumb = SDL_GAMEPAD_BUTTON_LEFT_STICK,
        RightThumb = SDL_GAMEPAD_BUTTON_RIGHT_STICK,

        LeftShoulder = SDL_GAMEPAD_BUTTON_LEFT_SHOULDER,
        RightShoulder = SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER,

        A = SDL_GAMEPAD_BUTTON_SOUTH,
        B = SDL_GAMEPAD_BUTTON_EAST,
        X = SDL_GAMEPAD_BUTTON_WEST,
        Y = SDL_GAMEPAD_BUTTON_NORTH
    };

    enum class KeyState
    {
        Up,
        Down,
        Pressed
    };

    enum class Stick
    {
        Left,
        Right
    };

    class Controller
    {
    public:
        explicit Controller(uint8_t controllerIndex);
        ~Controller();

        Controller(const Controller&) = delete;
        Controller& operator=(const Controller&) = delete;
        Controller(Controller&&) = delete;
        Controller& operator=(Controller&&) = delete;

        bool isDown(ControllerButton button) const;
        bool isPressed(ControllerButton button) const;
        bool isUp(ControllerButton button) const;

        void update();

        b2Vec2 getLeftThumb() const;
        b2Vec2 getRightThumb() const;

    private:
        class ControllerImpl;
        std::unique_ptr<ControllerImpl> m_pImpl;
    };
}

#endif
