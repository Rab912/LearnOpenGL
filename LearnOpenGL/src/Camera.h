#pragma once

#include <glm/glm.hpp>

enum CameraMotion
{
    FORWARD    = 0,
    LEFT       = 1,
    BACKWARD   = 2,
    RIGHT      = 3,
    UP         = 4,
    DOWN       = 5,
};

class Camera
{
public:
    Camera(float fov, float aspectRatio, float nearClip, float farClip);

    void processKeyboardInput(CameraMotion movement, float timeStep);
    void processMouseMovementInput(float yawDelta, float pitchDelta);
    void processMouseScrollInput(float scrollAmount);

    const glm::mat4& getView() const { return m_view; }
    const glm::mat4& getProjection() const { return m_projection; }

    glm::vec3 cameraPosition() { return m_cameraPosition; }
    glm::vec3 cameraFront() { return m_cameraFront; }
    glm::vec3 cameraRight() { return glm::normalize(glm::cross(m_cameraFront, m_worldUp)); }
    glm::vec3 cameraUp() { return glm::cross(cameraRight(), m_cameraFront); }

private:
    void updateView();
    void updateProjection();

private:
    float m_fov;
    float m_aspectRatio;
    float m_nearClip;
    float m_farClip;

    float m_yaw = -90.0f;
    float m_pitch = 0.0f;

    glm::mat4 m_view;
    glm::mat4 m_projection;

    glm::vec3 m_cameraFront = { 0.0f, 0.0f, -1.0f };
    glm::vec3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };

    glm::vec3 m_worldUp = { 0.0f, 1.0f, 0.0f };
};