#include "Input.hpp"


namespace Soft
{
    GLFWwindow *Input::working_window_ = nullptr;

    void Input::set_working_window(GLFWwindow *_window)
    {
        working_window_ = _window;
    }

    bool Input::key_pressed(Key _key)
    {
        int keyState = glfwGetKey(working_window_,(int)_key);
        return keyState == GLFW_PRESS || keyState == GLFW_REPEAT;
    }

    bool Input::key_released(Key _key)
    {
        int keyState = glfwGetKey(working_window_,(int)_key);
        return keyState == GLFW_RELEASE;
    }

    bool Input::key_down(Key _key)
    {
        int keyState = glfwGetKey(working_window_,(int)_key);
        return keyState == GLFW_PRESS && keyState != GLFW_REPEAT;
    }

    bool Input::mouse_button_pressed(MouseButton _button)
    {
        int buttonState = glfwGetMouseButton(working_window_,(int)_button);

        return buttonState == GLFW_PRESS || buttonState == GLFW_REPEAT;
    }

    bool Input::mouse_button_released(MouseButton _button)
    {
        int buttonState = glfwGetMouseButton(working_window_,(int)_button);

        return buttonState == GLFW_RELEASE;
    }

    float Input::get_mouse_x()
    {
        auto [x, y] = get_mouse_position();
        return (float)x;
    }

    float Input::get_mouse_y()
    {
        auto [x, y] = get_mouse_position();
        return (float)y;
    }

    std::pair<float, float> Input::get_mouse_position()
    {
        double x, y;
        glfwGetCursorPos(working_window_, &x, &y);
        return std::pair<float, float>((float)x, float(y));
    }
}