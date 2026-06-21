#include "Scene.h"
#include <algorithm>

Scene::Scene(const std::string& name) : m_name(name) {}

Entity& Scene::createEntity(const std::string& name) {
    auto entity = std::make_shared<Entity>();
    entity->name = name;
    m_entities.push_back(entity);
    return *entity;
}

void Scene::removeEntity(const std::string& name) {
    m_entities.erase(
        std::remove_if(m_entities.begin(), m_entities.end(),
            [&name](const std::shared_ptr<Entity>& e) {
                return e->name == name;
            }),
        m_entities.end()
    );
}

Entity* Scene::findEntity(const std::string& name) {
    for (auto& e : m_entities) {
        if (e->name == name) return e.get();
    }
    return nullptr;
}