#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

#include "Player.h"
#include "Map.h"
#include "Renderer.h"

class Game {
public:
    Game(int width, int height, const std::string& title);
    ~Game();

    void Run();

private:
    // Window properties
    GLFWwindow* m_Window;
    int m_Width;
    int m_Height;
    std::string m_Title;

    // Game components
    std::unique_ptr<Player> m_Player;
    std::unique_ptr<Map> m_Map;
    std::unique_ptr<Renderer> m_Renderer;

    // Time tracking
    float m_DeltaTime;
    float m_LastFrame;

    // Input handling
    void ProcessInput();
    
    // Callbacks
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    
    // Setup functions
    void InitWindow();
    void InitGame();
};