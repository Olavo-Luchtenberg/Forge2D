#pragma once
#include "Entity.h"
#include <vector>
#include <memory>
#include <string>

class Scene {
public:
    Scene(const std::string& name);
    virtual ~Scene() = default;

    virtual void init() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void destroy() = 0;

    Entity& createEntity(const std::string& name);
    void removeEntity(const std::string& name);
    Entity* findEntity(const std::string& name);

    const std::string& getName() const { return m_name; }
    std::vector<std::shared_ptr<Entity>>& getEntities() { return m_entities; }

private:
    std::string m_name;
    std::vector<std::shared_ptr<Entity>> m_entities;
};