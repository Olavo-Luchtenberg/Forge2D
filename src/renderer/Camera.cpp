#include "Camera.h"

Camera::Camera(float left, float right, float bottom, float top) {
    m_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_view = glm::mat4(1.0f);
    m_viewProjection = m_projection * m_view;
}

void Camera::setPosition(const glm::vec3& position) {
    m_position = position;
    recalculate();
}

void Camera::setZoom(float zoom) {
    m_zoom = zoom;
    recalculate();
}

void Camera::recalculate() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position);
    transform = glm::scale(transform, glm::vec3(m_zoom, m_zoom, 1.0f));
    m_view = glm::inverse(transform);
    m_viewProjection = m_projection * m_view;
}