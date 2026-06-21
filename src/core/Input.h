#pragma once
#include <SDL.h>
#include <glm/glm.hpp>
#include <unordered_map>

class Input {
public:
    static void update();
    static void processEvent(const SDL_Event& event);

    static bool isKeyDown(SDL_Keycode key);
    static bool isKeyPressed(SDL_Keycode key);
    static bool isKeyReleased(SDL_Keycode key);

    static bool isMouseButtonDown(int button);
    static glm::vec2 getMousePosition();

private:
    static std::unordered_map<SDL_Keycode, bool> s_currentKeys;
    static std::unordered_map<SDL_Keycode, bool> s_previousKeys;
    static std::unordered_map<int, bool> s_mouseButtons;
    static glm::vec2 s_mousePosition;
};