#pragma once

#include "Texture.h"

class Sprite {
public:
    Sprite(Texture* texture, SDL_Rect* rect) noexcept;
    Sprite(Texture* texture, uint32_t x, uint32_t y, uint32_t width, uint32_t height) noexcept;

    [[nodiscard]] Texture* getTexture() const noexcept;
    [[nodiscard]] SDL_Rect* getRect() const noexcept;

    SDL_RendererFlip flipMode = SDL_FLIP_NONE;

private:
    const Texture* m_texture;
    std::unique_ptr<SDL_Rect> m_rect;
};
