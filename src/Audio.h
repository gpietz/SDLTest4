#pragma once

#include <SDL2/SDL_mixer.h>
#include <iostream>

class Audio {
public:
    Audio();
    ~Audio();

    void playStreamed(const char* filename);
};
