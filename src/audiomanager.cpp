#include "../include/audiomanager.h"
#include <curses.h>
using namespace std;

AudioManager::AudioManager() {}

AudioManager::~AudioManager() {
    cleanup();
}

void AudioManager::init() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "SDL_mixer could not initialize! Mix Error: " << Mix_GetError() << endl;
    }
}

void AudioManager::cleanup() {
    // Free all loaded SFX
    for (auto const& [key, chunk] : sfxMap) {
        Mix_FreeChunk(chunk);
    }
    sfxMap.clear();

    if (currentMusic != nullptr) {
        Mix_FreeMusic(currentMusic);
    }

    Mix_Quit();
    SDL_Quit();
}

void AudioManager::playMusic(const string& filePath) {
    // If music is already playing, stop it
    stopMusic();

    // Load new music
    currentMusic = Mix_LoadMUS(filePath.c_str());
    if (currentMusic == nullptr) {
        cout << "Failed to load music! Mix Error: " << Mix_GetError() << endl;
        return;
    }

    // Play music (-1 means loop forever)
    Mix_PlayMusic(currentMusic, -1);
}

void AudioManager::stopMusic() {
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
    if (currentMusic != nullptr) {
        Mix_FreeMusic(currentMusic);
        currentMusic = nullptr;
    }
}

void AudioManager::playSFX(const string& filePath) {
    // Check if we already loaded this file
    if (sfxMap.find(filePath) == sfxMap.end()) {
        // Not found, load it now
        Mix_Chunk* newSFX = Mix_LoadWAV(filePath.c_str());
        if (newSFX == nullptr) {
             cout << "Failed to load SFX! Mix Error: " << Mix_GetError() << endl;
             return;
        }
        sfxMap[filePath] = newSFX;
    }

    // Play it on the first free channel (don't loop, so 0)
    Mix_PlayChannel(-1, sfxMap[filePath], 0);
}