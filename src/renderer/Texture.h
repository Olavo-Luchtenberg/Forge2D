#pragma once
#include <glad/glad.h>
#include <string>

class Texture {
public:
    Texture(const std::string& path);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

private:
    unsigned int m_id;
    int m_width, m_height, m_channels;
};