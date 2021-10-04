#pragma once

#include "SDL2/SDL.h"
#include "core/Types.h"
#include <string>

class Texture {
public:
    bool loadFromFile(const char *filename, SDL_Renderer* renderer) noexcept;
    void unload() noexcept;

    static SDL_Texture* createTextureFromSurface(SDL_Surface* surface, SDL_Renderer* renderer) noexcept;

    [[nodiscard]] const std::string& getName() const noexcept;
    [[nodiscard]] uint32_t getWidth() const noexcept;
    [[nodiscard]] uint32_t getHeight() const noexcept;
    [[nodiscard]] SDL_Texture* getSdlTexture() const noexcept;

private:
    SDL_Texture* m_texture;
    std::string m_name;
    uint32_t m_width;
    uint32_t m_height;

    void reset() noexcept;
};
