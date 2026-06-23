#include "core/Window.h"
#include "core/SceneManager.h"
#include "core/Audio.h"
#include "editor/Editor.h"
#include "examples/platformer/PlatformerScene.h"
#include <SDL.h>
#include <glad/glad.h>
#include "examples/platformer/PlatformerScene.h"

int main(int argc, char* argv[]) {
    Window window("Forge2D - Platformer", 800, 600);
    if (!window.init()) return -1;

    Audio::init();
    Editor::init(window.getSDLWindow(), window.getGLContext());

    SceneManager::get().push(std::make_shared<PlatformerScene>());

    Uint32 lastTime = SDL_GetTicks();
    bool running = true;

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        window.pollEvents(running);

        SceneManager::get().update(deltaTime);

        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SceneManager::get().render();

        Editor::beginFrame();
        if (auto* scene = dynamic_cast<PlatformerScene*>(SceneManager::get().current()))
            scene->renderUI();
        Editor::render(SceneManager::get().current());
        Editor::endFrame();

        window.swapBuffers();
    }

    SceneManager::get().pop();
    Editor::shutdown();
    Audio::shutdown();
    return 0;
}