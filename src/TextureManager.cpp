#include <iostream>
#include "TextureManager.h"
#include "helpers/IoHelpers.h"

TextureManager::~TextureManager() {
    // release all our remaining textures before we go
    NameIndex::iterator i, begin = m_nameIndex.begin(), end = m_nameIndex.end();
    for (i = begin; i != end; ++i) {
        m_textures.dereference(i->second)->unload();
    }
}

HTexture TextureManager::getTexture(const char *name, const SDL_Renderer* renderer) {
    auto pair =std::make_pair(name, HTexture());
    auto rc = m_nameIndex.insert(pair);
    if (rc.second) {
        // this is a new insertion
        auto texture = m_textures.acquire(rc.first->second);
        if (!texture->loadFromFile(rc.first->first.c_str(), renderer)) {
            texture->unload();
            rc.first->second = HTexture();
        }
    }
    return rc.first->second;
}

//SDL_Surface* TextureManager::getTextureDirect(const char *name) {
//    auto textureHandle = getTexture(name);
//    return m_textures.dereference(textureHandle)->m_image;
//}

bool TextureManager::Texture::loadFromFile(const char* filename, const SDL_Renderer* renderer) noexcept {
    auto filepath = getFullResourcePath(filename);
    auto image_surface = IMG_Load(filepath.c_str());
    if (image_surface == nullptr) {
        std::cerr << "Failed loading image: " << filename << std::endl;
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return false;
    }

    m_name    = filename;
    m_width   = image_surface->clip_rect.w;
    m_height  = image_surface->clip_rect.h;
    m_surface = image_surface;
    std::cout << "Loaded image: " << filename << " (" << m_width << "x" << m_height << ")" << std::endl;

    if (renderer == nullptr) {
        m_surface = image_surface;
    } else {
        return createTextureFromSurface(renderer);
    }

    return true;
}

bool TextureManager::Texture::createTextureFromSurface(const SDL_Renderer *renderer) noexcept {
    if (renderer == nullptr || m_surface == nullptr) {
        return false;
    }

    m_texture = SDL_CreateTextureFromSurface(const_cast<SDL_Renderer*>(renderer), m_surface);
    if (m_texture == nullptr) {
        std::cerr << "Failed to crate a texure from surface!" << std::endl;
        std::cerr << "SDL_CreateTextureFromSurface error: " << SDL_GetError << std::endl;
        return false;
    } else {
        std::cout << "Created texture from surface: " << m_name << std::endl;
    }

    SDL_FreeSurface(m_surface);
    m_surface = nullptr;
    return true;
}

void TextureManager::Texture::unload() noexcept {
    if (m_surface != nullptr) {
        SDL_FreeSurface(m_surface);
        m_surface = nullptr;
    }
    if (m_texture != nullptr) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
    std::cout << "Unloaded image: " << m_name << std::endl;
    reset();
}

void TextureManager::Texture::reset() {
    m_name.clear();
    m_width   = 0;
    m_height  = 0;
    m_surface = nullptr;
    m_texture = nullptr;
}
