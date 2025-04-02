#pragma once

#include <glm/glm-master/glm-master/glm/glm.hpp>
#include <glm/glm-master/glm-master/glm/gtc/matrix_transform.hpp>

class Map;

class Player {
public:
    enum Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    Player(const glm::vec3& position);
    
    void Update(float deltaTime, const Map& map);
    void Move(Direction dir, float deltaTime, const Map& map);
    void Look(float xoffset, float yoffset);
    
    // Getters
    glm::vec3 GetPosition() const { return m_Position; }
    glm::vec3 GetFront() const { return m_Front; }
    glm::vec3 GetRight() const { return m_Right; }
    glm::vec3 GetUp() const { return m_Up; }
    float GetYaw() const { return m_Yaw; }
    float GetPitch() const { return m_Pitch; }
    
    glm::mat4 GetViewMatrix() const;
    
private:
    // Position and orientation
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;
    
    // Euler angles
    float m_Yaw;
    float m_Pitch;
    
    // Camera options
    float m_MovementSpeed;
    float m_MouseSensitivity;
    
    // Update vectors based on Euler angles
    void UpdateCameraVectors();
    
    // Collision detection with walls
    bool CheckCollision(const glm::vec3& newPosition, const Map& map) const;
};