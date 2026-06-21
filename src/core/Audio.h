#pragma once
#include <SDL_mixer.h>
#include <string>
#include <unordered_map>

class Audio {
public:
    static bool init();
    static void shutdown();

    static void loadSound(const std::string& name, const std::string& path);
    static void loadMusic(const std::string& name, const std::string& path);

    static void playSound(const std::string& name, int loops = 0);
    static void playMusic(const std::string& name, int loops = -1);
    static void stopMusic();

    static void setMusicVolume(int volume);
    static void setSoundVolume(int volume);

private:
    static std::unordered_map<std::string, Mix_Chunk*> s_sounds;
    static std::unordered_map<std::string, Mix_Music*> s_music;
};