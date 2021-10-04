#include "SdlHelpers.h"

SDL_Rect SdlHelpers::createRect(int x, int y, int width, int height) noexcept {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    return rect;
}

SDL_Rect* SdlHelpers::createRectNew(int x, int y, int width, int height) noexcept {
    auto rect = new SDL_Rect();
    rect->x = x;
    rect->y = y;
    rect->w = width;
    rect->h = height;
    return rect;
}

SDL_Rect* SdlHelpers::createRectNew(const SDL_Rect* rect) noexcept {
    auto new_rect = new SDL_Rect();
    if (rect != nullptr) {
        new_rect->x = rect->x;
        new_rect->y = rect->y;
        new_rect->w = rect->w;
        new_rect->h = rect->h;
    }
    return new_rect;
}

bool SdlHelpers::copyRect(SDL_Rect *source, SDL_Rect *target) noexcept {
    if (source == nullptr || target == nullptr) {
        return false;
    }
    target->x = source->x;
    target->y = source->y;
    target->w = source->w;
    target->h = source->h;
    return true;
}
