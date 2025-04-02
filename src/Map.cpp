#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>

Map::Map(const std::string& filename) {
    try {
        LoadMap(filename);
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading map: " << e.what() << std::endl;
        std::cerr << "Falling back to test map." << std::endl;
        CreateTestMap();
    }
}

void Map::LoadMap(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open map file: " + filename);
    }
    
    // For our prototype, we'll just create a simple map
    // In a real game, we would parse the level data from the file
    CreateTestMap();
    
    // Initialize collision grid
    m_Width = 20;
    m_Height = 20;
    m_CollisionGrid.resize(m_Width, std::vector<bool>(m_Height, false));
    
    // Set collision for walls
    for (const auto& sector : m_Sectors) {
        for (const auto& wall : sector.walls) {
            // Simplified collision grid - mark the grid cells occupied by walls
            int startX = static_cast<int>(wall.start.x);
            int startZ = static_cast<int>(wall.start.y);
            int endX = static_cast<int>(wall.end.x);
            int endZ = static_cast<int>(wall.end.y);
            
            // Ensure within grid bounds
            startX = std::max(0, std::min(startX, m_Width - 1));
            startZ = std::max(0, std::min(startZ, m_Height - 1));
            endX = std::max(0, std::min(endX, m_Width - 1));
            endZ = std::max(0, std::min(endZ, m_Height - 1));
            
            // Mark wall cells
            for (int x = startX; x <= endX; ++x) {
                for (int z = startZ; z <= endZ; ++z) {
                    if (x < m_Width && z < m_Height) {
                        m_CollisionGrid[x][z] = true;
                    }
                }
            }
        }
    }
}

void Map::CreateTestMap() {
    // Create a simple test map: a room with 4 walls
    Sector room;
    room.floorHeight = 0.0f;
    room.ceilingHeight = 3.0f;
    room.floorTextureId = 0;
    room.ceilingTextureId = 0;
    
    // Add walls (rectangle)
    // Wall 1: bottom
    room.walls.push_back({
        glm::vec2(0.0f, 0.0f),     // start (x, z)
        glm::vec2(10.0f, 0.0f),    // end (x, z)
        3.0f,                       // height
        0                           // textureId
    });
    
    // Wall 2: right
    room.walls.push_back({
        glm::vec2(10.0f, 0.0f),    // start (x, z)
        glm::vec2(10.0f, 10.0f),   // end (x, z)
        3.0f,                       // height
        1                           // textureId
    });
    
    // Wall 3: top
    room.walls.push_back({
        glm::vec2(10.0f, 10.0f),   // start (x, z)
        glm::vec2(0.0f, 10.0f),    // end (x, z)
        3.0f,                       // height
        0                           // textureId
    });
    
    // Wall 4: left
    room.walls.push_back({
        glm::vec2(0.0f, 10.0f),    // start (x, z)
        glm::vec2(0.0f, 0.0f),     // end (x, z)
        3.0f,                       // height
        1                           // textureId
    });
    
    // Add a pillar in the middle
    room.walls.push_back({
        glm::vec2(4.0f, 4.0f),     // start (x, z)
        glm::vec2(6.0f, 4.0f),     // end (x, z)
        3.0f,                       // height
        2                           // textureId
    });
    
    room.walls.push_back({
        glm::vec2(6.0f, 4.0f),     // start (x, z)
        glm::vec2(6.0f, 6.0f),     // end (x, z)
        3.0f,                       // height
        2                           // textureId
    });
    
    room.walls.push_back({
        glm::vec2(6.0f, 6.0f),     // start (x, z)
        glm::vec2(4.0f, 6.0f),     // end (x, z)
        3.0f,                       // height
        2                           // textureId
    });
    
    room.walls.push_back({
        glm::vec2(4.0f, 6.0f),     // start (x, z)
        glm::vec2(4.0f, 4.0f),     // end (x, z)
        3.0f,                       // height
        2                           // textureId
    });
    
    m_Sectors.push_back(room);
}

bool Map::IsWallAt(float x, float z) const {
    // Convert world coordinates to grid coordinates
    int gridX = static_cast<int>(x);
    int gridZ = static_cast<int>(z);
    
    // Check bounds
    if (gridX < 0 || gridX >= m_Width || gridZ < 0 || gridZ >= m_Height) {
        return true; // Consider out-of-bounds as walls
    }
    
    return m_CollisionGrid[gridX][gridZ];
}