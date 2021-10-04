#pragma once

#include <SDL2/SDL.h>

class SdlHelpers {
public:
    static SDL_Rect createRect(int x, int y, int width, int height) noexcept;
    static SDL_Rect* createRectNew(int x, int y, int width, int height) noexcept;
    static SDL_Rect* createRectNew(const SDL_Rect* rect) noexcept;
    static bool copyRect(SDL_Rect* source, SDL_Rect* target) noexcept;
};
