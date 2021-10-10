#include "Sprite.h"
#include "core/Assert.h"
#include "helpers/SdlHelpers.h"

const char* RefError = "Invalid reference to texture object provided!";

Sprite::Sprite(Texture *texture, SDL_Rect* rect) noexcept {
    M_Assert(texture != nullptr, RefError);
    m_texture = texture;
    m_rect = std::unique_ptr<SDL_Rect>(rect);
}

Sprite::Sprite(Texture *texture, uint32_t x, uint32_t y, uint32_t width, uint32_t height) noexcept {
    M_Assert(texture != nullptr, RefError);
    m_texture = texture;
    m_rect = std::unique_ptr<SDL_Rect>(SdlHelpers::createRectNew(x, y, width, height));
}

Texture* Sprite::getTexture() const noexcept {
    return const_cast<Texture*>(m_texture);
}

SDL_Rect* Sprite::getRect() const noexcept {
    return m_rect.get();
}
