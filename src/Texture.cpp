#include "Texture.h"
#include <iostream>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb-master/stb-master/stb_image.h>

Texture::Texture(const std::string& path) 
    : m_TextureId(0), m_Width(0), m_Height(0), m_Channels(0) {
    
    // Generate texture
    glGenTextures(1, &m_TextureId);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Load image
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 0);
    
    if (data) {
        GLenum format;
        if (m_Channels == 1)
            format = GL_RED;
        else if (m_Channels == 3)
            format = GL_RGB;
        else if (m_Channels == 4)
            format = GL_RGBA;
        else {
            stbi_image_free(data);
            throw std::runtime_error("Unsupported number of channels: " + std::to_string(m_Channels));
        }
        
        // Create texture
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
        
        // Create a default checkerboard pattern as a fallback
        const int checkerSize = 16;
        const int size = checkerSize * 8; // 8x8 checker pattern
        
        unsigned char* checkerData = new unsigned char[size * size * 3];
        
        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                int idx = (y * size + x) * 3;
                
                bool isEvenRow = (y / checkerSize) % 2 == 0;
                bool isEvenCol = (x / checkerSize) % 2 == 0;
                
                if (isEvenRow == isEvenCol) {
                    // White square
                    checkerData[idx] = 255;     // R
                    checkerData[idx + 1] = 255; // G
                    checkerData[idx + 2] = 255; // B
                } else {
                    // Magenta square (to make it obvious it's a missing texture)
                    checkerData[idx] = 255;     // R
                    checkerData[idx + 1] = 0;   // G
                    checkerData[idx + 2] = 255; // B
                }
            }
        }
        
        // Create fallback texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, checkerData);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        delete[] checkerData;
        m_Width = size;
        m_Height = size;
        m_Channels = 3;
    }
    
    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_TextureId);
}

void Texture::Bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
}