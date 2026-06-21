#pragma once
#include "renderer/Texture.h"
#include "renderer/Shader.h"
#include <SDL_mixer.h>
#include <memory>
#include <unordered_map>
#include <string>

class AssetManager {
public:
    static AssetManager& get();

    std::shared_ptr<Texture> loadTexture(const std::string& name, const std::string& path);
    std::shared_ptr<Texture> getTexture(const std::string& name);

    std::shared_ptr<Shader> loadShader(const std::string& name, const std::string& vertSrc, const std::string& fragSrc);
    std::shared_ptr<Shader> getShader(const std::string& name);

    void loadSound(const std::string& name, const std::string& path);
    Mix_Chunk* getSound(const std::string& name);

    void clear();

private:
    AssetManager() = default;

    std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
    std::unordered_map<std::string, std::shared_ptr<Shader>>  m_shaders;
    std::unordered_map<std::string, Mix_Chunk*>               m_sounds;
};