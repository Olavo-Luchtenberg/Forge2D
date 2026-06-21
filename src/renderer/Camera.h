#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(float left, float right, float bottom, float top);

    void setPosition(const glm::vec3& position);
    void setZoom(float zoom);

    const glm::mat4& getViewProjectionMatrix() const { return m_viewProjection; }

private:
    void recalculate();

    glm::mat4 m_projection;
    glm::mat4 m_view;
    glm::mat4 m_viewProjection;

    glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
    float m_zoom = 1.0f;
};