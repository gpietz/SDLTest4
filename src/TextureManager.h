#pragma once

#include <map>
#include <vector>
#include <string>
#include <SDL_image.h>
#include "core/HandleManager.h"
#include "helpers/SdlHelpers.h"

HANDLE_DECL(Texture);

class TextureManager {
    struct Texture {
        std::string  m_name;
        uint32_t     m_width;
        uint32_t     m_height;
        SDL_Rect     m_position;
        SDL_Surface* m_surface;
        SDL_Texture* m_texture;

        bool loadFromFile(const char* filename, const SDL_Renderer* renderer = nullptr) noexcept;
        bool createTextureFromSurface(const SDL_Renderer* renderer) noexcept;
        void unload() noexcept;

    private:
        void reset();
    };

    typedef HandleManager<Texture, HTexture> HTextureMgr;

    // case-insensitive string comparison predicate
    struct string_less_compare {
        bool operator() (const std::string& left, const std::string& right) const {
            size_t max_size = std::max(left.size(), right.size());
            return _strnicmp(left.c_str(), right.c_str(), max_size);
        }
    };

    typedef std::map<std::string, HTexture, string_less_compare> NameIndex;

    HTextureMgr m_textures;
    NameIndex   m_nameIndex;

public:
    TextureManager() { }
    ~TextureManager();

    // texture management
    HTexture getTexture(const char* name, const SDL_Renderer* renderer = nullptr);
    void deleteTexture(HTexture texture);

    // texture query
    const std::string& getName(HTexture texture) const {
        return m_textures.dereference(texture)->m_name;
    }
    int getWidth(HTexture texture) const {
        return m_textures.dereference(texture)->m_width;
    }
    int getHeight(HTexture texture) const {
        return m_textures.dereference(texture)->m_height;
    }
    nop_unique_ptr<SDL_Surface> getSurface(HTexture texture) const {
        return nop_unique_ptr<SDL_Surface>(m_textures.dereference(texture)->m_surface);
    }
    nop_unique_ptr<SDL_Texture> getTexture(HTexture texture) const {
        return nop_unique_ptr<SDL_Texture>(m_textures.dereference(texture)->m_texture);
    }
    nop_unique_ptr<SDL_Texture> getTextureByName(const char* name) {
        auto texture_handle = getTexture(name, nullptr);
        return nop_unique_ptr<SDL_Texture>(m_textures.dereference(texture_handle)->m_texture);
    }
    std::unique_ptr<SDL_Rect> getRect(HTexture texture) {
        auto texture_obj = m_textures.dereference(texture);
        return std::unique_ptr<SDL_Rect>(SdlHelpers::createRectNew(0, 0, texture_obj->m_width, texture_obj->m_height));
    }
};
