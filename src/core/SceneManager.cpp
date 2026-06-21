#include "SceneManager.h"

SceneManager& SceneManager::get() {
    static SceneManager instance;
    return instance;
}

void SceneManager::push(std::shared_ptr<Scene> scene) {
    m_scenes.push(scene);
    m_scenes.top()->init();
}

void SceneManager::pop() {
    if (!m_scenes.empty()) {
        m_scenes.top()->destroy();
        m_scenes.pop();
    }
}

void SceneManager::replace(std::shared_ptr<Scene> scene) {
    pop();
    push(scene);
}

Scene* SceneManager::current() {
    if (m_scenes.empty()) return nullptr;
    return m_scenes.top().get();
}

void SceneManager::update(float deltaTime) {
    if (current()) current()->update(deltaTime);
}

void SceneManager::render() {
    if (current()) current()->render();
}