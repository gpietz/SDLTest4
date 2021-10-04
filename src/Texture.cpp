#include "Texture.h"
#include "core/Assert.h"
#include "helpers/IoHelpers.h"
#include "SDL_image.h"

bool Texture::loadFromFile(const char* filename, SDL_Renderer* renderer) noexcept {
    M_Assert(filename != nullptr, "loadFromFile requires a filename - nullptr provided!");
    M_Assert(renderer != nullptr, "loadFromFile requires reference to SDL_Renderer!");

    auto filepath = getFullResourcePath(filename);
    auto image_surface = IMG_Load(filepath.c_str());
    if (image_surface == nullptr) {
        std::cerr << "Failed loading image: " << filename << std::endl;
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return false;
    }

    m_texture = createTextureFromSurface(image_surface, renderer);
    if (m_texture == nullptr) {
        return false;
    }

    m_name   = filename;
    m_width  = image_surface->w;
    m_height = image_surface->h;

    SDL_FreeSurface(image_surface);
    image_surface = nullptr;
    return true;
}

void Texture::Texture::unload() noexcept {
    if (m_texture != nullptr) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
    std::cout << "Unloaded image: " << m_name << std::endl;
    reset();
}

void Texture::Texture::reset() noexcept {
    m_name.clear();
    m_width   = 0;
    m_height  = 0;
    m_texture = nullptr;
}

SDL_Texture* Texture::createTextureFromSurface(SDL_Surface* surface, SDL_Renderer* renderer) noexcept {
    M_Assert(renderer != nullptr, "createTextureFromSurface requires reference to SDL_Renderer!");
    auto texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        std::cerr << "Failed to create texture from surface!" << std::endl;
        std::cerr << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << std::endl;
    }
    return texture;
}

const std::string &Texture::getName() const noexcept {
    return m_name;
}

uint32_t Texture::getWidth() const noexcept {
    return m_width;
}

uint32_t Texture::getHeight() const noexcept {
    return m_height;
}

SDL_Texture *Texture::getSdlTexture() const noexcept {
    return m_texture;
}
