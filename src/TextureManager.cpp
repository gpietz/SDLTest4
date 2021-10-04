#include "TextureManager.h"

TextureManager::TextureManager() = default;

TextureManager::~TextureManager() {
    // release all our remaining textures before we go
    for (auto &m_textureName : m_textureNames) {
        m_textures.dereference(m_textureName.second)->unload();
    }
}

Texture* TextureManager::getTexture(const char *name, SDL_Renderer *renderer) {
    auto pair = std::make_pair(name, HTexture());
    auto rc = m_textureNames.insert(pair);
    Texture* texture;
    if (rc.second) {
        // this is a new insertion - texture doesn't exists yet
        texture = m_textures.acquire(rc.first->second);
        if (!texture->loadFromFile(rc.first->first.c_str(), renderer)) {
            texture->unload();
            rc.first->second = HTexture();
            texture = nullptr;
        }
    } else {
        texture = m_textures.dereference(rc.first->second);
    }
    return texture;
}
