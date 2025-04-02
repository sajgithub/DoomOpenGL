#pragma once

#include <glad/glad.h>
#include <glm/glm-master/glm-master/glm/glm.hpp>
#include <vector>
#include <memory>

#include "Player.h"
#include "Map.h"
#include "ShaderManager.h"
#include "Texture.h"

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();
    
    void Render(const Player& player, const Map& map);
    void ResizeViewport(int width, int height);
    
private:
    int m_Width;
    int m_Height;
    
    // Shader and texture management
    std::unique_ptr<ShaderManager> m_ShaderManager;
    std::vector<std::unique_ptr<Texture>> m_Textures;
    
    // OpenGL objects
    GLuint m_WallVAO;
    GLuint m_WallVBO;
    GLuint m_FloorVAO;
    GLuint m_FloorVBO;
    
    // Projection matrix
    glm::mat4 m_Projection;
    
    // Setup
    void InitRendering();
    void LoadTextures();
    
    // Render components
    void RenderWalls(const Player& player, const Map& map);
    void RenderFloorAndCeiling(const Player& player, const Map& map);
};