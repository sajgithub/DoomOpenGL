#pragma once

#include <vector>
#include <string>
#include <glm/glm-master/glm-master/glm/glm.hpp>

// Define a wall segment
struct Wall {
    glm::vec2 start;    // Start point (x, z)
    glm::vec2 end;      // End point (x, z)
    float height;       // Wall height
    int textureId;      // Texture ID to use
};

// Define a sector (room)
struct Sector {
    std::vector<Wall> walls;
    float floorHeight;
    float ceilingHeight;
    int floorTextureId;
    int ceilingTextureId;
};

class Map {
public:
    Map(const std::string& filename);
    
    // Query methods
    const std::vector<Sector>& GetSectors() const { return m_Sectors; }
    bool IsWallAt(float x, float z) const;
    
private:
    std::vector<Sector> m_Sectors;
    int m_Width;
    int m_Height;
    
    // For simplified collision detection
    std::vector<std::vector<bool>> m_CollisionGrid;
    
    // Load map from file
    void LoadMap(const std::string& filename);
    
    // Create a simple test map (used when file loading fails)
    void CreateTestMap();
};