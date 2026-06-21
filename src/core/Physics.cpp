#include "Physics.h"
#include <algorithm>
#include <cmath>

bool Physics::checkAABB(const AABB& a, const AABB& b) {
    return (a.position.x < b.position.x + b.size.x &&
            a.position.x + a.size.x > b.position.x &&
            a.position.y < b.position.y + b.size.y &&
            a.position.y + a.size.y > b.position.y);
}

CollisionResult Physics::resolveAABB(const AABB& a, const AABB& b) {
    CollisionResult result;

    float dx = (a.position.x + a.size.x / 2) - (b.position.x + b.size.x / 2);
    float dy = (a.position.y + a.size.y / 2) - (b.position.y + b.size.y / 2);

    float overlapX = (a.size.x / 2 + b.size.x / 2) - std::abs(dx);
    float overlapY = (a.size.y / 2 + b.size.y / 2) - std::abs(dy);

    if (overlapX <= 0 || overlapY <= 0) return result;

    result.collided = true;

    if (overlapX < overlapY) {
        result.normal = { dx > 0 ? 1.0f : -1.0f, 0.0f };
        result.depth = overlapX;
    } else {
        result.normal = { 0.0f, dy > 0 ? 1.0f : -1.0f };
        result.depth = overlapY;
    }

    return result;
}

AABB Physics::getAABB(const glm::vec3& position, const glm::vec2& size, const Collider& collider) {
    return {
        { position.x - size.x / 2 + collider.offset.x,
          position.y - size.y / 2 + collider.offset.y },
        collider.size
    };
}