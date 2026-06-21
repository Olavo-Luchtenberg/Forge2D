#pragma once
#include "core/Scene.h"
#include <SDL.h>

class Editor {
public:
    static bool init(SDL_Window* window, SDL_GLContext context);
    static void shutdown();

    static void processEvent(SDL_Event* event);
    static void beginFrame();
    static void endFrame();
    static void render(Scene* scene);

private:
    static void drawEntityPanel(Scene* scene);
    static void drawInspector(Entity* entity);

    static Entity* s_selected;
};