#pragma once

#include <glad/glad.h>
#include <string>
#include <unordered_map>

class ShaderManager {
public:
    ShaderManager() = default;
    ~ShaderManager();
    
    void LoadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    GLuint GetShader(const std::string& name) const;
    
private:
    std::unordered_map<std::string, GLuint> m_Shaders;
    
    GLuint CompileShader(GLenum type, const std::string& source);
    std::string ReadFile(const std::string& path);
};