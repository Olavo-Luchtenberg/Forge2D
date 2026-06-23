#pragma once
#include "core/Scene.h"
#include "core/Physics.h"
#include "core/AssetManager.h"
#include "renderer/Camera.h"
#include <imgui.h>
#include <memory>
#include <vector>

struct Particle {
    glm::vec3 position;
    glm::vec2 velocity;
    glm::vec4 color;
    float life;
    float maxLife;
    float size;
};

class PlatformerScene : public Scene {
public:
    PlatformerScene(int level = 1);

    void init() override;
    void update(float deltaTime) override;
    void render() override;
    void destroy() override;
    void renderUI();

private:
    std::shared_ptr<Camera> m_camera;
    unsigned int m_VAO, m_VBO, m_EBO;

    int m_score = 0;
    int m_level;
    bool m_gameOver    = false;
    bool m_levelClear  = false;
    float m_stateTimer = 0.0f;

    float m_animTimer  = 0.0f;
    int   m_animFrame  = 0;
    bool  m_facingRight = true;

    std::vector<Particle> m_particles;

    void setupQuad();
    void drawEntity(Entity& entity);
    void drawPlayerAnimated(Entity& entity, int frame, bool flipX);
    void applyGravity(float deltaTime);
    void resolveCollisions();
    void checkCoins();
    void checkEnemies();
    void checkLevelClear();
    void spawnExplosion(float x, float y);
    void spawnConfetti();
    void updateParticles(float deltaTime);
    void renderParticles();
    void loadLevel(int level);
    void createPlatform(const std::string& name, float x, float y, float w, float h);
    void createCoin(const std::string& name, float x, float y);
    void createEnemy(const std::string& name, float x, float y, float speed = 80.0f);

    int m_totalCoins = 0;
    int m_coinsCollected = 0;
};