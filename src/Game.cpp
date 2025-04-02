#include "Game.h"
#include <iostream>
#include <stdexcept>

// Static member initialization for callbacks
namespace {
    Game* currentGameInstance = nullptr;
}

Game::Game(int width, int height, const std::string& title)
    : m_Width(width), m_Height(height), m_Title(title),
      m_DeltaTime(0.0f), m_LastFrame(0.0f) {
    
    currentGameInstance = this;
    
    InitWindow();
    InitGame();
}

Game::~Game() {
    // Clean up GLFW
    glfwTerminate();
}

void Game::InitWindow() {
    // Initialize GLFW
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    
    // Make OpenGL context current
    glfwMakeContextCurrent(m_Window);
    
    // Set callbacks
    glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(m_Window, MouseCallback);
    
    // Capture mouse
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Load OpenGL function pointers with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
}

void Game::InitGame() {
    // Create player
    m_Player = std::make_unique<Player>(glm::vec3(2.0f, 0.0f, 2.0f));
    
    // Create map
    m_Map = std::make_unique<Map>("maps/level1.txt");
    
    // Create renderer
    m_Renderer = std::make_unique<Renderer>(m_Width, m_Height);
}

void Game::Run() {
    while (!glfwWindowShouldClose(m_Window)) {
        // Calculate delta time
        float currentFrame = static_cast<float>(glfwGetTime());
        m_DeltaTime = currentFrame - m_LastFrame;
        m_LastFrame = currentFrame;
        
        // Process input
        ProcessInput();
        
        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Update player
        m_Player->Update(m_DeltaTime, *m_Map);
        
        // Render the scene
        m_Renderer->Render(*m_Player, *m_Map);
        
        // Swap buffers and poll events
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}

void Game::ProcessInput() {
    // Close window on ESC
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_Window, true);
    }
    
    // Player movement
    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS) {
        m_Player->Move(Player::FORWARD, m_DeltaTime, *m_Map);
    }
    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS) {
        m_Player->Move(Player::BACKWARD, m_DeltaTime, *m_Map);
    }
    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS) {
        m_Player->Move(Player::LEFT, m_DeltaTime, *m_Map);
    }
    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS) {
        m_Player->Move(Player::RIGHT, m_DeltaTime, *m_Map);
    }
}

void Game::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    if (currentGameInstance) {
        currentGameInstance->m_Width = width;
        currentGameInstance->m_Height = height;
        currentGameInstance->m_Renderer->ResizeViewport(width, height);
    }
}

void Game::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    static bool firstMouse = true;
    static double lastX = 0.0, lastY = 0.0;
    
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = static_cast<float>(xpos - lastX);
    float yoffset = static_cast<float>(lastY - ypos); // Reversed: y coordinates go bottom to top
    
    lastX = xpos;
    lastY = ypos;
    
    if (currentGameInstance && currentGameInstance->m_Player) {
        currentGameInstance->m_Player->Look(xoffset, yoffset);
    }
}