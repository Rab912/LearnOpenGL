#pragma once

#include <iostream>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    Camera(float fov, float aspectRatio, float nearClip, float farClip)
        : m_fov(fov), m_aspectRatio(aspectRatio), m_nearClip(nearClip), m_farClip(farClip),
        m_projection(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
    {        
        updateView();
    }

    void processKeyboardInput(CameraMotion movement, float timeStep)
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

    void processMouseMovementInput(float yawDelta, float pitchDelta)
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

    void processMouseScrollInput(float scrollAmount)
    {
        m_fov -= (float)scrollAmount;

        m_fov = std::clamp(m_fov, 1.0f, 179.999f);

        updateProjection();
    }

    const glm::mat4& getView() const { return m_view; }

    const glm::mat4& getProjection() const { return m_projection; }

    glm::vec3 cameraFront() { return m_cameraFront; }

    glm::vec3 cameraRight() { return glm::normalize(glm::cross(m_cameraFront, m_worldUp)); }

    glm::vec3 cameraUp() { return glm::cross(cameraRight(), m_cameraFront); }

private:
    void updateView()
    {
        m_view = glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, cameraUp());
    }

    void updateProjection()
    {
        m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
    }

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