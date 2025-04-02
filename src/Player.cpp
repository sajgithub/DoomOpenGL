#include "Player.h"
#include "Map.h"
#include <glm\glm-master\glm-master\glm\gtc\matrix_transform.hpp>
#include <algorithm>

Player::Player(const glm::vec3& position)
    : m_Position(position),
      m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      m_WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
      m_Yaw(-90.0f), // Default orientation: looking down -Z
      m_Pitch(0.0f),
      m_MovementSpeed(2.5f),
      m_MouseSensitivity(0.1f) {
    
    UpdateCameraVectors();
}

void Player::Update(float deltaTime, const Map& map) {
    // This method would handle any per-frame updates for the player
    // Currently empty as movement is handled in Move method
}

void Player::Move(Direction dir, float deltaTime, const Map& map) {
    float velocity = m_MovementSpeed * deltaTime;
    glm::vec3 newPosition = m_Position;
    
    switch (dir) {
        case FORWARD:
            newPosition += m_Front * velocity;
            break;
        case BACKWARD:
            newPosition -= m_Front * velocity;
            break;
        case LEFT:
            newPosition -= m_Right * velocity;
            break;
        case RIGHT:
            newPosition += m_Right * velocity;
            break;
    }
    
    // Only update if no collision
    if (!CheckCollision(newPosition, map)) {
        m_Position = newPosition;
    }
}

void Player::Look(float xoffset, float yoffset) {
    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;
    
    m_Yaw += xoffset;
    m_Pitch += yoffset;
    
    // Constrain pitch to avoid flipping
    m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);
    
    UpdateCameraVectors();
}

glm::mat4 Player::GetViewMatrix() const {
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Player::UpdateCameraVectors() {
    // Calculate new front vector from Euler angles
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);
    
    // Recalculate right and up vectors
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

bool Player::CheckCollision(const glm::vec3& newPosition, const Map& map) const {
    // Player collision radius (distance from center to edge)
    const float playerRadius = 0.3f;
    
    // Check if the player's new position would collide with a wall
    // We check multiple points around the player to simulate a collision cylinder
    
    // Check center point
    if (map.IsWallAt(newPosition.x, newPosition.z)) {
        return true;
    }
    
    // Check points around the player (simulating a collision cylinder)
    for (int i = 0; i < 8; i++) {
        float angle = i * glm::pi<float>() / 4.0f; // 8 points around the circle
        float checkX = newPosition.x + playerRadius * cos(angle);
        float checkZ = newPosition.z + playerRadius * sin(angle);
        
        if (map.IsWallAt(checkX, checkZ)) {
            return true;
        }
    }
    
    // No collision detected
    return false;
}