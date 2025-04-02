#pragma once

#include <glad/glad.h>
#include <string>

class Texture {
public:
    Texture(const std::string& path);
    ~Texture();
    
    void Bind(unsigned int slot = 0) const;
    
    GLuint GetId() const { return m_TextureId; }
    
private:
    GLuint m_TextureId;
    int m_Width;
    int m_Height;
    int m_Channels;
};