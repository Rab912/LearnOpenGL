#include <iostream>
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

Camera::Camera(float fov, float aspectRatio, float nearClip, float farClip)
    : m_fov(fov), m_aspectRatio(aspectRatio), m_nearClip(nearClip), m_farClip(farClip),
    m_projection(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
{
    updateView();
}

void Camera::processKeyboardInput(CameraMotion movement, float timeStep)
{
    float delta = 10.0f * timeStep;

    switch (movement)
    {
    case FORWARD:
        m_cameraPosition += delta * m_cameraFront;
        break;

    case LEFT:
        m_cameraPosition -= delta * cameraRight();
        break;

    case BACKWARD:
        m_cameraPosition -= delta * m_cameraFront;
        break;

    case RIGHT:
        m_cameraPosition += delta * cameraRight();
        break;

    case UP:
        m_cameraPosition += delta * cameraUp();
        break;

    case DOWN:
        m_cameraPosition -= delta * cameraUp();
        break;
    }

    updateView();
}

void Camera::processMouseMovementInput(float yawDelta, float pitchDelta)
{
    m_yaw += yawDelta;
    m_pitch -= pitchDelta;

    m_pitch = std::clamp(m_pitch, -89.999f, 89.999f);

    glm::vec3 direction;
    direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_cameraFront = glm::normalize(direction);

    updateView();
}

void Camera::processMouseScrollInput(float scrollAmount)
{
    m_fov -= (float)scrollAmount;

    m_fov = std::clamp(m_fov, 1.0f, 179.999f);

    updateProjection();
}

void Camera::updateView()
{
    m_view = glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, cameraUp());
}

void Camera::updateProjection()
{
    m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
}