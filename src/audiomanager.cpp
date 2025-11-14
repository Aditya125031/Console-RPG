#include "../include/audiomanager.h"
#include <curses.h>


#include "../include/game.h"

AudioManager::AudioManager() {}

AudioManager::~AudioManager() {
    cleanup();
}

void AudioManager::init() {
    // Initialize SDL Audio only
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
    }

    // Initialize SDL_mixer for MP3 and OGG support
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer could not initialize! Mix Error: " << Mix_GetError() << std::endl;
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

void AudioManager::playMusic(const std::string& filePath, Game& world) {
    // If music is already playing, stop it
    stopMusic();

    // Load new music
    currentMusic = Mix_LoadMUS(filePath.c_str());
    if (currentMusic == nullptr) {
        world.add_log_message(string("Failed to load music! Mix Error: ") + Mix_GetError());
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

void AudioManager::playSFX(const std::string& filePath, Game& world) {
    // Check if we already loaded this file
    if (sfxMap.find(filePath) == sfxMap.end()) {
        // Not found, load it now
        Mix_Chunk* newSFX = Mix_LoadWAV(filePath.c_str());
        if (newSFX == nullptr) {
             world.add_log_message(string("Failed to load SFX! Mix Error: ") + Mix_GetError());
             return;
        }
        sfxMap[filePath] = newSFX;
    }

    // Play it on the first free channel (don't loop, so 0)
    Mix_PlayChannel(-1, sfxMap[filePath], 0);
}