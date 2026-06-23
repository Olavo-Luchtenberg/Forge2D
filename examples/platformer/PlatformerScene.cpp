#include "PlatformerScene.h"
#include "core/Input.h"
#include "core/Audio.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

const std::string vertexSrc = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec2 aTexCoord;
    out vec2 TexCoord;
    uniform mat4 uTransform;
    uniform mat4 uViewProjection;
    void main() {
        gl_Position = uViewProjection * uTransform * vec4(aPos, 0.0, 1.0);
        TexCoord = aTexCoord;
    }
)";

const std::string fragmentSrc = R"(
    #version 330 core
    out vec4 FragColor;
    in vec2 TexCoord;
    uniform sampler2D uTexture;
    uniform vec4 uColor;
    void main() {
        FragColor = texture(uTexture, TexCoord) * uColor;
    }
)";

PlatformerScene::PlatformerScene(int level) : Scene("PlatformerScene"), m_level(level) {}

void PlatformerScene::init() {
    auto& assets = AssetManager::get();
    assets.loadShader("platformer_sprite", vertexSrc, fragmentSrc);
    assets.loadTexture("player_sheet", "../examples/platformer/assets/player_sheet.png");
    assets.loadTexture("platform",     "../examples/platformer/assets/platform.png");
    assets.loadTexture("coin",         "../examples/platformer/assets/coin.png");
    assets.loadTexture("enemy",        "../examples/platformer/assets/enemy.png");
    assets.loadTexture("bg",           "../examples/platformer/assets/bg.png");

    m_camera = std::make_shared<Camera>(0.0f, 800.0f, 0.0f, 600.0f);
    setupQuad();
    loadLevel(m_level);
}

void PlatformerScene::loadLevel(int level) {
    auto& player = createEntity("player");
    player.transform.position = { 100.0f, 400.0f, 0.0f };
    player.transform.size     = { 40.0f, 40.0f };
    player.sprite.texturePath = "player_sheet";
    player.hasCollider        = true;
    player.collider.size      = { 40.0f, 40.0f };
    player.collider.isStatic  = false;

    float enemySpeed = 60.0f + level * 20.0f;

    if (level == 1) {
        createPlatform("ground", 400.0f, 50.0f,  800.0f, 20.0f);
        createPlatform("plat1",  200.0f, 150.0f, 150.0f, 20.0f);
        createPlatform("plat2",  500.0f, 250.0f, 150.0f, 20.0f);
        createPlatform("plat3",  300.0f, 350.0f, 150.0f, 20.0f);
        createPlatform("plat4",  650.0f, 400.0f, 100.0f, 20.0f);
        createCoin("coin1", 200.0f, 190.0f);
        createCoin("coin2", 500.0f, 290.0f);
        createCoin("coin3", 300.0f, 390.0f);
        createCoin("coin4", 650.0f, 440.0f);
        createEnemy("enemy1", 500.0f, 290.0f, enemySpeed);
    } else if (level == 2) {
        createPlatform("ground", 400.0f, 50.0f,  800.0f, 20.0f);
        createPlatform("plat1",  150.0f, 130.0f, 120.0f, 20.0f);
        createPlatform("plat2",  400.0f, 200.0f, 120.0f, 20.0f);
        createPlatform("plat3",  650.0f, 270.0f, 120.0f, 20.0f);
        createPlatform("plat4",  300.0f, 340.0f, 120.0f, 20.0f);
        createPlatform("plat5",  550.0f, 410.0f, 120.0f, 20.0f);
        createCoin("coin1", 150.0f, 170.0f);
        createCoin("coin2", 400.0f, 240.0f);
        createCoin("coin3", 650.0f, 310.0f);
        createCoin("coin4", 300.0f, 380.0f);
        createCoin("coin5", 550.0f, 450.0f);
        createEnemy("enemy1", 400.0f, 240.0f, enemySpeed);
        createEnemy("enemy2", 650.0f, 310.0f, enemySpeed);
    } else if (level == 3) {
        createPlatform("ground", 400.0f, 50.0f,  800.0f, 20.0f);
        createPlatform("plat1",  100.0f, 120.0f, 100.0f, 20.0f);
        createPlatform("plat2",  300.0f, 190.0f, 100.0f, 20.0f);
        createPlatform("plat3",  500.0f, 260.0f, 100.0f, 20.0f);
        createPlatform("plat4",  700.0f, 330.0f, 100.0f, 20.0f);
        createPlatform("plat5",  500.0f, 400.0f, 100.0f, 20.0f);
        createPlatform("plat6",  300.0f, 470.0f, 100.0f, 20.0f);
        createCoin("coin1", 100.0f, 160.0f);
        createCoin("coin2", 300.0f, 230.0f);
        createCoin("coin3", 500.0f, 300.0f);
        createCoin("coin4", 700.0f, 370.0f);
        createCoin("coin5", 500.0f, 440.0f);
        createCoin("coin6", 300.0f, 510.0f);
        createEnemy("enemy1", 300.0f, 230.0f, enemySpeed);
        createEnemy("enemy2", 500.0f, 300.0f, enemySpeed);
        createEnemy("enemy3", 700.0f, 370.0f, enemySpeed);
    }

    m_totalCoins = 0;
    m_coinsCollected = 0;
    for (auto& e : getEntities())
        if (e->name.find("coin") != std::string::npos)
            m_totalCoins++;
}

void PlatformerScene::update(float deltaTime) {
    if (m_gameOver || m_levelClear) {
        m_stateTimer += deltaTime;
        updateParticles(deltaTime);

        if (m_gameOver && Input::isKeyPressed(SDLK_r)) {
            getEntities().clear();
            m_particles.clear();
            m_gameOver   = false;
            m_stateTimer = 0.0f;
            m_coinsCollected = 0;
            loadLevel(m_level);
        }

        if (m_levelClear && m_stateTimer > 2.0f) {
            if (Input::isKeyPressed(SDLK_RETURN) || Input::isKeyPressed(SDLK_SPACE)) {
                getEntities().clear();
                m_particles.clear();
                m_levelClear = false;
                m_stateTimer = 0.0f;
                m_coinsCollected = 0;
                m_level = m_level < 3 ? m_level + 1 : 1;
                loadLevel(m_level);
            }
        }
        return;
    }

    auto* player = findEntity("player");
    if (!player) return;

    float speed = 200.0f;
    float jumpForce = 450.0f;

    if (Input::isKeyDown(SDLK_LEFT))  player->collider.velocity.x = -speed;
    else if (Input::isKeyDown(SDLK_RIGHT)) player->collider.velocity.x = speed;
    else player->collider.velocity.x *= 0.8f;

    if ((Input::isKeyDown(SDLK_SPACE) || Input::isKeyDown(SDLK_UP)) && player->collider.onGround) {
        player->collider.velocity.y = jumpForce;
        player->collider.onGround = false;
    }

    // animacao
    m_animTimer += deltaTime;
    if (!player->collider.onGround) {
        m_animFrame = 3;
    } else if (std::abs(player->collider.velocity.x) > 10.0f) {
        if (m_animTimer > 0.15f) {
            m_animFrame = (m_animFrame == 1) ? 2 : 1;
            m_animTimer = 0.0f;
        }
        m_facingRight = player->collider.velocity.x > 0;
    } else {
        m_animFrame = 0;
    }

    applyGravity(deltaTime);

    player->transform.position.x += player->collider.velocity.x * deltaTime;
    player->transform.position.y += player->collider.velocity.y * deltaTime;
    player->transform.position.x = glm::clamp(player->transform.position.x, 20.0f, 780.0f);

    if (player->transform.position.y < -100.0f) {
        m_gameOver = true;
        spawnExplosion(player->transform.position.x, 300.0f);
    }

    resolveCollisions();
    checkCoins();
    checkEnemies();
    checkLevelClear();
    updateParticles(deltaTime);

    float enemySpeed = 60.0f + m_level * 20.0f;
    for (auto& entity : getEntities()) {
        if (entity->name.find("enemy") == std::string::npos) continue;
        entity->collider.velocity.x = enemySpeed * (entity->sprite.color.r > 0.9f ? 1.0f : -1.0f);
        entity->transform.position.x += entity->collider.velocity.x * deltaTime;
        if (entity->transform.position.x > 750.0f || entity->transform.position.x < 50.0f)
            entity->sprite.color.r = entity->sprite.color.r > 0.9f ? 0.5f : 1.0f;
    }
}

void PlatformerScene::applyGravity(float deltaTime) {
    for (auto& entity : getEntities()) {
        if (!entity->hasCollider || entity->collider.isStatic) continue;
        if (entity->name.find("coin") != std::string::npos) continue;
        entity->collider.velocity.y -= Physics::GRAVITY * deltaTime;
    }
}

void PlatformerScene::resolveCollisions() {
    auto& entities = getEntities();
    auto* player = findEntity("player");
    if (player) player->collider.onGround = false;

    for (size_t i = 0; i < entities.size(); i++) {
        if (!entities[i]->hasCollider || entities[i]->collider.isStatic) continue;
        if (entities[i]->name.find("coin") != std::string::npos) continue;

        entities[i]->collider.size = entities[i]->transform.size;

        for (size_t j = 0; j < entities.size(); j++) {
            if (i == j || !entities[j]->hasCollider || !entities[j]->collider.isStatic) continue;

            entities[j]->collider.size = entities[j]->transform.size;

            AABB a = Physics::getAABB(entities[i]->transform.position, entities[i]->transform.size, entities[i]->collider);
            AABB b = Physics::getAABB(entities[j]->transform.position, entities[j]->transform.size, entities[j]->collider);

            CollisionResult result = Physics::resolveAABB(a, b);

            if (result.collided) {
                entities[i]->transform.position.x += result.normal.x * result.depth;
                entities[i]->transform.position.y += result.normal.y * result.depth;

                if (result.normal.y > 0.5f) {
                    entities[i]->collider.velocity.y = 0.0f;
                    if (entities[i]->name == "player")
                        entities[i]->collider.onGround = true;
                } else if (result.normal.y < -0.5f) {
                    entities[i]->collider.velocity.y = 0.0f;
                } else {
                    entities[i]->collider.velocity.x = 0.0f;
                }
            }
        }
    }
}

void PlatformerScene::checkCoins() {
    auto* player = findEntity("player");
    if (!player) return;

    AABB playerAABB = Physics::getAABB(player->transform.position, player->transform.size, player->collider);

    for (auto& entity : getEntities()) {
        if (entity->name.find("coin") == std::string::npos) continue;
        if (!entity->active) continue;

        AABB coinAABB = Physics::getAABB(entity->transform.position, entity->transform.size, entity->collider);
        if (Physics::checkAABB(playerAABB, coinAABB)) {
            entity->active = false;
            m_score += 10 * m_level;
            m_coinsCollected++;
        }
    }
}

void PlatformerScene::checkEnemies() {
    auto* player = findEntity("player");
    if (!player) return;

    AABB playerAABB = Physics::getAABB(player->transform.position, player->transform.size, player->collider);

    for (auto& entity : getEntities()) {
        if (entity->name.find("enemy") == std::string::npos) continue;

        AABB enemyAABB = Physics::getAABB(entity->transform.position, entity->transform.size, entity->collider);
        if (Physics::checkAABB(playerAABB, enemyAABB)) {
            m_gameOver = true;
            spawnExplosion(player->transform.position.x, player->transform.position.y);
        }
    }
}

void PlatformerScene::checkLevelClear() {
    if (m_totalCoins > 0 && m_coinsCollected >= m_totalCoins) {
        m_levelClear = true;
        spawnConfetti();
    }
}

void PlatformerScene::spawnExplosion(float x, float y) {
    for (int i = 0; i < 40; i++) {
        Particle p;
        p.position = { x, y, 0.0f };
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float spd = 100.0f + rand() % 300;
        p.velocity  = { cos(angle) * spd, sin(angle) * spd };
        p.color     = { 1.0f, (rand() % 100) / 100.0f, 0.0f, 1.0f };
        p.life      = 0.5f + (rand() % 100) / 100.0f;
        p.maxLife   = p.life;
        p.size      = 4.0f + rand() % 8;
        m_particles.push_back(p);
    }
}

void PlatformerScene::spawnConfetti() {
    for (int i = 0; i < 80; i++) {
        Particle p;
        p.position = { (float)(rand() % 800), 600.0f, 0.0f };
        p.velocity  = { -50.0f + rand() % 100, -100.0f - (float)(rand() % 200) };
        p.color     = { (rand()%100)/100.0f, (rand()%100)/100.0f, (rand()%100)/100.0f, 1.0f };
        p.life      = 1.5f + (rand() % 100) / 100.0f;
        p.maxLife   = p.life;
        p.size      = 6.0f + rand() % 8;
        m_particles.push_back(p);
    }
}

void PlatformerScene::updateParticles(float deltaTime) {
    for (auto& p : m_particles) {
        p.position.x += p.velocity.x * deltaTime;
        p.position.y += p.velocity.y * deltaTime;
        p.velocity.y -= 200.0f * deltaTime;
        p.life -= deltaTime;
        p.color.a = p.life / p.maxLife;
    }
    m_particles.erase(
        std::remove_if(m_particles.begin(), m_particles.end(),
            [](const Particle& p) { return p.life <= 0; }),
        m_particles.end()
    );
}

void PlatformerScene::renderParticles() {
    for (auto& p : m_particles) {
        Entity e;
        e.transform.position = p.position;
        e.transform.size     = { p.size, p.size };
        e.sprite.texturePath = "coin";
        e.sprite.color       = p.color;
        drawEntity(e);
    }
}

void PlatformerScene::renderUI() {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.6f);
    ImGui::Begin("HUD", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Nivel: %d", m_level);
    ImGui::Text("Score: %d", m_score);
    ImGui::Text("Moedas: %d/%d", m_coinsCollected, m_totalCoins);
    ImGui::End();

    if (m_gameOver) {
        ImGui::SetNextWindowPos(ImVec2(250, 220), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(300, 120), ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::Begin("GameOver", nullptr, ImGuiWindowFlags_NoDecoration);
        ImGui::SetWindowFontScale(2.0f);
        ImGui::TextColored(ImVec4(1,0.2f,0.2f,1), "  GAME OVER");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::Text("Score final: %d", m_score);
        ImGui::TextColored(ImVec4(1,1,0,1), "  Pressione R para reiniciar");
        ImGui::End();
    }

    if (m_levelClear) {
        ImGui::SetNextWindowPos(ImVec2(220, 220), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(360, 120), ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.85f);
        ImGui::Begin("LevelClear", nullptr, ImGuiWindowFlags_NoDecoration);
        ImGui::SetWindowFontScale(2.0f);
        ImGui::TextColored(ImVec4(0,1,0.3f,1), "  NIVEL COMPLETO!");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::Text("Score: %d", m_score);
        if (m_stateTimer > 2.0f)
            ImGui::TextColored(ImVec4(1,1,0,1), "  ENTER para proximo nivel");
        ImGui::End();
    }
}

void PlatformerScene::render() {
    Entity bgEntity;
    bgEntity.transform.position = { 400.0f, 300.0f, 0.0f };
    bgEntity.transform.size     = { 800.0f, 600.0f };
    bgEntity.sprite.texturePath = "bg";
    bgEntity.sprite.color       = { 1.0f, 1.0f, 1.0f, 1.0f };
    drawEntity(bgEntity);

    for (auto& entity : getEntities()) {
        if (!entity->active) continue;
        if (entity->name == "player")
            drawPlayerAnimated(*entity, m_animFrame, !m_facingRight);
        else
            drawEntity(*entity);
    }

    renderParticles();
}

void PlatformerScene::destroy() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    AssetManager::get().clear();
}

void PlatformerScene::setupQuad() {
    float vertices[] = {
        -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.0f, 1.0f
    };
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void PlatformerScene::createPlatform(const std::string& name, float x, float y, float w, float h) {
    auto& e = createEntity(name);
    e.transform.position = { x, y, 0.0f };
    e.transform.size     = { w, h };
    e.sprite.texturePath = "platform";
    e.hasCollider        = true;
    e.collider.size      = { w, h };
    e.collider.isStatic  = true;
}

void PlatformerScene::createCoin(const std::string& name, float x, float y) {
    auto& e = createEntity(name);
    e.transform.position = { x, y, 0.0f };
    e.transform.size     = { 20.0f, 20.0f };
    e.sprite.texturePath = "coin";
    e.hasCollider        = true;
    e.collider.size      = { 20.0f, 20.0f };
    e.collider.isTrigger = true;
}

void PlatformerScene::createEnemy(const std::string& name, float x, float y, float speed) {
    auto& e = createEntity(name);
    e.transform.position = { x, y, 0.0f };
    e.transform.size     = { 40.0f, 40.0f };
    e.sprite.texturePath = "enemy";
    e.hasCollider        = true;
    e.collider.size      = { 40.0f, 40.0f };
    e.collider.isStatic  = false;
    e.sprite.color       = { 1.0f, 0.3f, 0.3f, 1.0f };
}

void PlatformerScene::drawPlayerAnimated(Entity& entity, int frame, bool flipX) {
    auto shader  = AssetManager::get().getShader("platformer_sprite");
    auto texture = AssetManager::get().getTexture("player_sheet");
    if (!shader || !texture) return;

    float u0 = (frame * 32.0f) / 128.0f;
    float u1 = u0 + 32.0f / 128.0f;
    if (flipX) std::swap(u0, u1);

    float vertices[] = {
        -0.5f, -0.5f,  u0, 0.0f,
         0.5f, -0.5f,  u1, 0.0f,
         0.5f,  0.5f,  u1, 1.0f,
        -0.5f,  0.5f,  u0, 1.0f
    };
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, entity.transform.position);
    transform = glm::scale(transform, glm::vec3(entity.transform.size, 1.0f));

    shader->bind();
    shader->setMat4("uViewProjection", m_camera->getViewProjectionMatrix());
    shader->setMat4("uTransform", transform);
    shader->setVec4("uColor", entity.sprite.color);
    texture->bind(0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void PlatformerScene::drawEntity(Entity& entity) {
    auto shader  = AssetManager::get().getShader("platformer_sprite");
    auto texture = AssetManager::get().getTexture(entity.sprite.texturePath);
    if (!shader || !texture) return;

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, entity.transform.position);
    transform = glm::rotate(transform, glm::radians(entity.transform.rotation), glm::vec3(0, 0, 1));
    transform = glm::scale(transform, glm::vec3(entity.transform.size, 1.0f));

    shader->bind();
    shader->setMat4("uViewProjection", m_camera->getViewProjectionMatrix());
    shader->setMat4("uTransform", transform);
    shader->setVec4("uColor", entity.sprite.color);
    texture->bind(0);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}