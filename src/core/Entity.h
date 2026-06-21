#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Physics.h"

struct Transform {
    glm::vec3 position = { 0.0f, 0.0f, 0.0f };
    glm::vec2 size     = { 100.0f, 100.0f };
    float rotation     = 0.0f;
};

struct SpriteComponent {
    std::string texturePath;
    glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
};

struct Entity {
    std::string name;
    Transform transform;
    SpriteComponent sprite;
    Collider collider;
    bool hasCollider = false;
    bool active = true;
};