#include "Renderer.h"
#include <glm/glm-master/glm-master/glm/gtc/matrix_transform.hpp>
#include <iostream>

Renderer::Renderer(int width, int height)
    : m_Width(width), m_Height(height) {
    
    // Create projection matrix
    m_Projection = glm::perspective(glm::radians(45.0f), 
                                  static_cast<float>(width) / static_cast<float>(height),
                                  0.1f, 100.0f);
    
    // Initialize shader and resources
    m_ShaderManager = std::make_unique<ShaderManager>();
    
    // Load shaders
    m_ShaderManager->LoadShader("basic", "shaders/basic.vert", "shaders/basic.frag");
    
    // Initialize rendering
    InitRendering();
    
    // Load textures
    LoadTextures();
}

Renderer::~Renderer() {
    // Clean up OpenGL objects
    glDeleteVertexArrays(1, &m_WallVAO);
    glDeleteBuffers(1, &m_WallVBO);
    glDeleteVertexArrays(1, &m_FloorVAO);
    glDeleteBuffers(1, &m_FloorVBO);
}

void Renderer::InitRendering() {
    // Create vertex arrays and buffers for walls
    glGenVertexArrays(1, &m_WallVAO);
    glGenBuffers(1, &m_WallVBO);
    
    // Create vertex arrays and buffers for floor/ceiling
    glGenVertexArrays(1, &m_FloorVAO);
    glGenBuffers(1, &m_FloorVBO);
}

void Renderer::LoadTextures() {
    // Load wall textures
    m_Textures.push_back(std::make_unique<Texture>("resources/wall1.jpg"));
    m_Textures.push_back(std::make_unique<Texture>("resources/wall2.png"));
    
    // Load floor texture
    m_Textures.push_back(std::make_unique<Texture>("resources/floor.jpg"));
}

void Renderer::Render(const Player& player, const Map& map) {
    // Get shader
    GLuint shader = m_ShaderManager->GetShader("basic");
    glUseProgram(shader);
    
    // Set view matrix based on player position and orientation
    glm::mat4 view = player.GetViewMatrix();
    
    // Set uniforms
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &m_Projection[0][0]);
    
    // Render walls
    RenderWalls(player, map);
    
    // Render floor and ceiling
    RenderFloorAndCeiling(player, map);
}

void Renderer::RenderWalls(const Player& player, const Map& map) {
    GLuint shader = m_ShaderManager->GetShader("basic");
    
    // Bind wall VAO
    glBindVertexArray(m_WallVAO);
    
    // For each sector in the map
    for (const auto& sector : map.GetSectors()) {
        // For each wall in the sector
        for (const auto& wall : sector.walls) {
            // Create model matrix for this wall
            glm::mat4 model = glm::mat4(1.0f);
            
            // Set wall texture
            m_Textures[wall.textureId]->Bind(0);
            glUniform1i(glGetUniformLocation(shader, "textureSampler"), 0);
            
            // Create wall vertices
            float vertices[] = {
                // Positions          // Texture coords
                wall.start.x, 0.0f, wall.start.y,  0.0f, 0.0f,
                wall.start.x, wall.height, wall.start.y,  0.0f, 1.0f,
                wall.end.x, wall.height, wall.end.y,  1.0f, 1.0f,
                
                wall.start.x, 0.0f, wall.start.y,  0.0f, 0.0f,
                wall.end.x, wall.height, wall.end.y,  1.0f, 1.0f,
                wall.end.x, 0.0f, wall.end.y,  1.0f, 0.0f
            };
            
            // Upload vertices
            glBindBuffer(GL_ARRAY_BUFFER, m_WallVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            
            // Position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            
            // Texture coord attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            
            // Set model matrix
            glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &model[0][0]);
            
            // Draw wall
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
    
    // Unbind VAO
    glBindVertexArray(0);
}

void Renderer::RenderFloorAndCeiling(const Player& player, const Map& map) {
    // Similar implementation to RenderWalls but for floor and ceiling
    // This is a simplified version
    
    GLuint shader = m_ShaderManager->GetShader("basic");
    
    // Bind floor VAO
    glBindVertexArray(m_FloorVAO);
    
    // For each sector in the map
    for (const auto& sector : map.GetSectors()) {
        // Set floor texture
        m_Textures[sector.floorTextureId]->Bind(0);
        glUniform1i(glGetUniformLocation(shader, "textureSampler"), 0);
        
        // Create model matrix for floor
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, sector.floorHeight, 0.0f));
        
        // Set model matrix
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &model[0][0]);
        
        // Draw floor (simplified - in a real game, we would create proper floor geometry)
        // For now, just a placeholder
    }
    
    // Unbind VAO
    glBindVertexArray(0);
}

void Renderer::ResizeViewport(int width, int height) {
    m_Width = width;
    m_Height = height;
    
    // Update projection matrix
    m_Projection = glm::perspective(glm::radians(45.0f), 
                                  static_cast<float>(width) / static_cast<float>(height),
                                  0.1f, 100.0f);
}