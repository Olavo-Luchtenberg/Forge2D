#include "AssetManager.h"
#include <iostream>

AssetManager& AssetManager::get() {
    static AssetManager instance;
    return instance;
}

std::shared_ptr<Texture> AssetManager::loadTexture(const std::string& name, const std::string& path) {
    auto it = m_textures.find(name);
    if (it != m_textures.end()) return it->second;

    auto texture = std::make_shared<Texture>(path);
    m_textures[name] = texture;
    return texture;
}

std::shared_ptr<Texture> AssetManager::getTexture(const std::string& name) {
    auto it = m_textures.find(name);
    if (it != m_textures.end()) return it->second;
    std::cerr << "Textura nao encontrada: " << name << std::endl;
    return nullptr;
}

std::shared_ptr<Shader> AssetManager::loadShader(const std::string& name, const std::string& vertSrc, const std::string& fragSrc) {
    auto it = m_shaders.find(name);
    if (it != m_shaders.end()) return it->second;

    auto shader = std::make_shared<Shader>(vertSrc, fragSrc);
    m_shaders[name] = shader;
    return shader;
}

std::shared_ptr<Shader> AssetManager::getShader(const std::string& name) {
    auto it = m_shaders.find(name);
    if (it != m_shaders.end()) return it->second;
    std::cerr << "Shader nao encontrado: " << name << std::endl;
    return nullptr;
}

void AssetManager::loadSound(const std::string& name, const std::string& path) {
    if (m_sounds.count(name)) return;

    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (!chunk) {
        std::cerr << "Erro ao carregar som: " << path << std::endl;
        return;
    }
    m_sounds[name] = chunk;
}

Mix_Chunk* AssetManager::getSound(const std::string& name) {
    auto it = m_sounds.find(name);
    if (it != m_sounds.end()) return it->second;
    std::cerr << "Som nao encontrado: " << name << std::endl;
    return nullptr;
}

void AssetManager::clear() {
    m_textures.clear();
    m_shaders.clear();
    for (auto& [name, chunk] : m_sounds)
        Mix_FreeChunk(chunk);
    m_sounds.clear();
}