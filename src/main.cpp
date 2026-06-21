#include "core/Window.h"
#include "core/SceneManager.h"
#include "core/Audio.h"
#include "editor/Editor.h"
#include "GameScene.h"
#include <SDL.h>
#include <glad/glad.h>

int main(int argc, char* argv[]) {
    Window window("Engine", 800, 600);
    if (!window.init()) return -1;

    Audio::init();
    Editor::init(window.getSDLWindow(), window.getGLContext());

    SceneManager::get().push(std::make_shared<GameScene>());

    Uint32 lastTime = SDL_GetTicks();
    bool running = true;

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        window.pollEvents(running);

        SceneManager::get().update(deltaTime);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SceneManager::get().render();

        Editor::beginFrame();
        Editor::render(SceneManager::get().current());
        Editor::endFrame();

        window.swapBuffers();
    }

    SceneManager::get().pop();
    Editor::shutdown();
    Audio::shutdown();
    return 0;
}