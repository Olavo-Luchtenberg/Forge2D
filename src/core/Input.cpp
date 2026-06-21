#include "Input.h"

std::unordered_map<SDL_Keycode, bool> Input::s_currentKeys;
std::unordered_map<SDL_Keycode, bool> Input::s_previousKeys;
std::unordered_map<int, bool> Input::s_mouseButtons;
glm::vec2 Input::s_mousePosition = { 0.0f, 0.0f };

void Input::update() {
    s_previousKeys = s_currentKeys;
}

void Input::processEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN)
        s_currentKeys[event.key.keysym.sym] = true;
    if (event.type == SDL_KEYUP)
        s_currentKeys[event.key.keysym.sym] = false;
    if (event.type == SDL_MOUSEBUTTONDOWN)
        s_mouseButtons[event.button.button] = true;
    if (event.type == SDL_MOUSEBUTTONUP)
        s_mouseButtons[event.button.button] = false;
    if (event.type == SDL_MOUSEMOTION)
        s_mousePosition = { (float)event.motion.x, (float)event.motion.y };
}

bool Input::isKeyDown(SDL_Keycode key) {
    return s_currentKeys.count(key) && s_currentKeys[key];
}

bool Input::isKeyPressed(SDL_Keycode key) {
    return s_currentKeys.count(key) && s_currentKeys[key] &&
           !(s_previousKeys.count(key) && s_previousKeys[key]);
}

bool Input::isKeyReleased(SDL_Keycode key) {
    return !(s_currentKeys.count(key) && s_currentKeys[key]) &&
           s_previousKeys.count(key) && s_previousKeys[key];
}

bool Input::isMouseButtonDown(int button) {
    return s_mouseButtons.count(button) && s_mouseButtons[button];
}

glm::vec2 Input::getMousePosition() {
    return s_mousePosition;
}