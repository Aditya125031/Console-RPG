#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <string>
#include <map>
// Include SDL2_mixer. Adjust path depending on where you installed it.
// often it is <SDL2/SDL_mixer.h> or just <SDL_mixer.h>
#include <SDL_mixer.h> 
#include <SDL.h>
#include <iostream>
//#include "game.h"

class Game;

class AudioManager {
private:
    // Map to hold loaded sound effects so we don't reload them every time
    std::map<std::string, Mix_Chunk*> sfxMap;
    Mix_Music* currentMusic = nullptr;

public:
    AudioManager();
    ~AudioManager();

    void init();
    void cleanup();

    // For background music (MP3/OGG)
    void playMusic(const std::string& filePath, Game&);
    void stopMusic();

    // For sound effects (WAV)
    void playSFX(const std::string& filePath, Game&);
};

#endif