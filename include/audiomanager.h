#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <string>
#include <map>
#include <SDL_mixer.h> 
#include <SDL.h>
#include <iostream>
class AudioManager {
private:
    std::map<std::string, Mix_Chunk*> sfxMap;
    Mix_Music* currentMusic = nullptr;

public:
    AudioManager();
    ~AudioManager();

    void init();
    void cleanup();

    void playMusic(const std::string& filePath);
    void stopMusic();

    void playSFX(const std::string& filePath);
};

#endif