#pragma once
#include <glm/glm.hpp>

struct AABB {
    glm::vec2 position;
    glm::vec2 size;
};

struct CollisionResult {
    bool collided = false;
    glm::vec2 normal = { 0.0f, 0.0f };
    float depth = 0.0f;
};

struct Collider {
    glm::vec2 offset = { 0.0f, 0.0f };
    glm::vec2 size   = { 100.0f, 100.0f };
    bool isTrigger   = false;
    bool isStatic    = false;
};

class Physics {
public:
    static bool checkAABB(const AABB& a, const AABB& b);
    static CollisionResult resolveAABB(const AABB& a, const AABB& b);
    static AABB getAABB(const glm::vec3& position, const glm::vec2& size, const Collider& collider);
};