#pragma once
#include "core/Scene.h"
#include "core/Physics.h"
#include "core/AssetManager.h"
#include "core/ScriptEngine.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "renderer/Camera.h"
#include <memory>

class GameScene : public Scene {
public:
    GameScene();

    void init() override;
    void update(float deltaTime) override;
    void render() override;
    void destroy() override;

private:
    std::shared_ptr<Camera> m_camera;
    unsigned int m_VAO, m_VBO, m_EBO;

    void setupQuad();
    void drawEntity(Entity& entity);
    void resolveCollisions();
};