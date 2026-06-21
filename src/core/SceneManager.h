#pragma once
#include "Scene.h"
#include <memory>
#include <stack>

class SceneManager {
public:
    static SceneManager& get();

    void push(std::shared_ptr<Scene> scene);
    void pop();
    void replace(std::shared_ptr<Scene> scene);

    Scene* current();

    void update(float deltaTime);
    void render();

private:
    SceneManager() = default;
    std::stack<std::shared_ptr<Scene>> m_scenes;
};