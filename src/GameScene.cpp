#include "GameScene.h"
#include "core/Input.h"
#include "core/Audio.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

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

GameScene::GameScene() : Scene("GameScene") {}

void GameScene::init() {
    auto& assets = AssetManager::get();

    assets.loadShader("sprite", vertexSrc, fragmentSrc);
    assets.loadTexture("player_tex", "../assets/sprite.png");
    assets.loadSound("hit", "../assets/sound.wav");

    m_camera = std::make_shared<Camera>(0.0f, 800.0f, 0.0f, 600.0f);
    setupQuad();

    auto& player = createEntity("player");
    player.transform.position = { 400.0f, 300.0f, 0.0f };
    player.transform.size     = { 100.0f, 100.0f };
    player.sprite.texturePath = "player_tex";
    player.hasCollider        = true;
    player.collider.size      = { 100.0f, 100.0f };
    player.collider.isStatic  = false;

    auto& wall = createEntity("wall");
    wall.transform.position = { 600.0f, 300.0f, 0.0f };
    wall.transform.size     = { 100.0f, 300.0f };
    wall.sprite.texturePath = "player_tex";
    wall.sprite.color       = { 1.0f, 0.3f, 0.3f, 1.0f };
    wall.hasCollider        = true;
    wall.collider.size      = { 100.0f, 300.0f };
    wall.collider.isStatic  = true;

    ScriptEngine::init();
    ScriptEngine::bindEntity(findEntity("player"));
    ScriptEngine::loadScript("../assets/player.lua");
}

void GameScene::update(float deltaTime) {
    auto* player = findEntity("player");
    if (!player) return;

    float speed = 200.0f;
    if (Input::isKeyDown(SDLK_LEFT))  player->transform.position.x -= speed * deltaTime;
    if (Input::isKeyDown(SDLK_RIGHT)) player->transform.position.x += speed * deltaTime;
    if (Input::isKeyDown(SDLK_UP))    player->transform.position.y += speed * deltaTime;
    if (Input::isKeyDown(SDLK_DOWN))  player->transform.position.y -= speed * deltaTime;

    player->transform.position.x = glm::clamp(player->transform.position.x, 50.0f, 750.0f);
    player->transform.position.y = glm::clamp(player->transform.position.y, 50.0f, 550.0f);

    resolveCollisions();
    ScriptEngine::callUpdate(deltaTime);
}

void GameScene::resolveCollisions() {
    auto& entities = getEntities();

    for (size_t i = 0; i < entities.size(); i++) {
        if (!entities[i]->hasCollider || entities[i]->collider.isStatic) continue;

        entities[i]->collider.size = entities[i]->transform.size;

        for (size_t j = 0; j < entities.size(); j++) {
            if (i == j || !entities[j]->hasCollider) continue;

            entities[j]->collider.size = entities[j]->transform.size;

            AABB a = Physics::getAABB(entities[i]->transform.position, entities[i]->transform.size, entities[i]->collider);
            AABB b = Physics::getAABB(entities[j]->transform.position, entities[j]->transform.size, entities[j]->collider);

            CollisionResult result = Physics::resolveAABB(a, b);

            if (result.collided && !entities[j]->collider.isTrigger) {
                entities[i]->transform.position.x += result.normal.x * result.depth;
                entities[i]->transform.position.y += result.normal.y * result.depth;
                Audio::playSound("hit");
            }
        }
    }
}

void GameScene::render() {
    for (auto& entity : getEntities()) {
        if (entity->active)
            drawEntity(*entity);
    }
}

void GameScene::destroy() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    ScriptEngine::shutdown();
    AssetManager::get().clear();
}

void GameScene::setupQuad() {
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

void GameScene::drawEntity(Entity& entity) {
    auto shader  = AssetManager::get().getShader("sprite");
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