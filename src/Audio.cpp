#include <SDL_mixer.h>
#include "Audio.h"
#include "IoHelpers.h"

const int AUDIO_FLAGS = MIX_INIT_OGG | MIX_INIT_MOD;

Audio::Audio() {
    if (AUDIO_FLAGS != Mix_Init(AUDIO_FLAGS)) {
        std::cout << "Failed to initialize mixer!" << std::endl;
        std::cout << "Mix_Init: " << Mix_GetError() << std::endl;
        return;
    }
    std::cout << "Initialized mixer." << std::endl;
    std::cout << "Current directory: " << getCurrentDirectory() << std::endl;
}

Audio::~Audio() {
}

void Audio::playStreamed(const char *filename) {
    if (Mix_OpenAudio(44100,  MIX_DEFAULT_FORMAT, 2,  4096) == -1) {
        std::cerr << "Failed to open mixer with certain audio format!" << std::endl;
        std::cerr << "Mix_OpenAudio: " << Mix_GetError() << std::endl;
        return;
    }

    auto filename2 = getFullResourcePath(filename);
    auto *music = Mix_LoadMUS(filename2.c_str());
    if (music == NULL) {
        std::cerr << "Unable to load ogg file: " << filename << std::endl;
        std::cerr << "Mix_LoadMus: " << Mix_GetError() << std::endl;
        return;
    }

    if (Mix_PlayMusic(music, 0) == -1) {
        std::cerr << "Unable to play ogg file: " << Mix_GetError() << std::endl;
        return;
    }
}
