#include "Input.h"

Input::Input()
{
    pressed = new bool[keyboardMap.size()];
    for (int i = 0; i < keyboardMap.size(); ++i) {
        pressed[i] = false;
    }
}

Input::~Input()
{
    delete[] pressed;
}

void Input::ProcessInput(GLFWwindow* window)
{
    for (auto const& keyMap : keyboardMap) {
        if (glfwGetKey(window, keyMap.first) == GLFW_PRESS) {
            pressed[keyMap.second] = true;
        }
    }
}

unsigned char Input::WaitForInput(GLFWwindow* window)
{
    while (true) {
        for (auto const& keyMap: keyboardMap){
            if (pressed[keyMap.second]) {
                pressed[keyMap.second];
                return keyMap.second;
            }
        }
    }
}

bool Input::WasPressed(GLFWwindow* window, unsigned char regVal)
{
    if (pressed[regVal]) {
        pressed[regVal] = false;
        return true;
    }
    return false;
}
