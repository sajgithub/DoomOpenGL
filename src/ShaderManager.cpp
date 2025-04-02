#include "ShaderManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

ShaderManager::~ShaderManager() {
    // Delete all shaders
    for (auto& shader : m_Shaders) {
        glDeleteProgram(shader.second);
    }
}

void ShaderManager::LoadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) {
    try {
        // Read shader files
        std::string vertexCode = ReadFile(vertexPath);
        std::string fragmentCode = ReadFile(fragmentPath);
        
        // Compile shaders
        GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexCode);
        GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentCode);
        
        // Create shader program
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        
        // Check for linking errors
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
            throw std::runtime_error("Shader program linking failed: " + std::string(infoLog));
        }
        
        // Delete shaders (they are now linked into the program)
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        // Store the shader program
        m_Shaders[name] = program;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to load shader: " << e.what() << std::endl;
        
        // Create a fallback shader
        if (m_Shaders.find(name) == m_Shaders.end()) {
            const char* vertexSource = R"(
                #version 330 core
                layout (location = 0) in vec3 aPos;
                layout (location = 1) in vec2 aTexCoord;
                
                out vec2 TexCoord;
                
                uniform mat4 model;
                uniform mat4 view;
                uniform mat4 projection;
                
                void main() {
                    gl_Position = projection * view * model * vec4(aPos, 1.0);
                    TexCoord = aTexCoord;
                }
            )";
            
            const char* fragmentSource = R"(
                #version 330 core
                out vec4 FragColor;
                
                in vec2 TexCoord;
                
                uniform sampler2D textureSampler;
                
                void main() {
                    FragColor = texture(textureSampler, TexCoord);
                }
            )";
            
            // Compile fallback shaders
            GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
            GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
            
            // Create fallback program
            GLuint program = glCreateProgram();
            glAttachShader(program, vertexShader);
            glAttachShader(program, fragmentShader);
            glLinkProgram(program);
            
            // Store the fallback shader
            m_Shaders[name] = program;
            
            // Clean up
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }
    }
}

GLuint ShaderManager::GetShader(const std::string& name) const {
    auto it = m_Shaders.find(name);
    if (it != m_Shaders.end()) {
        return it->second;
    }
    
    std::cerr << "Shader not found: " << name << std::endl;
    return 0;
}

GLuint ShaderManager::CompileShader(GLenum type, const std::string& source) {
    // Create shader
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    
    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        std::string shaderType = (type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
        throw std::runtime_error(shaderType + " shader compilation failed: " + std::string(infoLog));
    }
    
    return shader;
}

std::string ShaderManager::ReadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}