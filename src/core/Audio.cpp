#include "Audio.h"
#include <iostream>

std::unordered_map<std::string, Mix_Chunk*> Audio::s_sounds;
std::unordered_map<std::string, Mix_Music*> Audio::s_music;

bool Audio::init() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Erro ao inicializar SDL_mixer: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void Audio::shutdown() {
    for (auto& [name, chunk] : s_sounds)
        Mix_FreeChunk(chunk);
    for (auto& [name, music] : s_music)
        Mix_FreeMusic(music);
    s_sounds.clear();
    s_music.clear();
    Mix_CloseAudio();
}

void Audio::loadSound(const std::string& name, const std::string& path) {
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (!chunk) {
        std::cerr << "Erro ao carregar som: " << path << " - " << Mix_GetError() << std::endl;
        return;
    }
    s_sounds[name] = chunk;
}

void Audio::loadMusic(const std::string& name, const std::string& path) {
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (!music) {
        std::cerr << "Erro ao carregar musica: " << path << " - " << Mix_GetError() << std::endl;
        return;
    }
    s_music[name] = music;
}

void Audio::playSound(const std::string& name, int loops) {
    auto it = s_sounds.find(name);
    if (it != s_sounds.end())
        Mix_PlayChannel(-1, it->second, loops);
}

void Audio::playMusic(const std::string& name, int loops) {
    auto it = s_music.find(name);
    if (it != s_music.end())
        Mix_PlayMusic(it->second, loops);
}

void Audio::stopMusic() {
    Mix_HaltMusic();
}

void Audio::setMusicVolume(int volume) {
    Mix_VolumeMusic(volume);
}

void Audio::setSoundVolume(int volume) {
    Mix_Volume(-1, volume);
}