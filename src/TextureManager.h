#pragma once

#include <map>
#include <vector>
#include <string>
#include <SDL_image.h>
#include "core/HandleManager.h"
#include "helpers/SdlHelpers.h"
#include "Texture.h"

HANDLE_DECL(Texture);

class TextureManager {
public:
    TextureManager();
    ~TextureManager();

    Texture* getTexture(const char* name, SDL_Renderer* renderer = nullptr);

private:
    // case-insensitive string comparison predicate
    struct string_less_compare {
        bool operator() (const std::string& left, const std::string& right) const {
            size_t max_size = std::max(left.size(), right.size());
            return _strnicmp(left.c_str(), right.c_str(), max_size);
        }
    };

    typedef HandleManager<Texture, HTexture> HTextureMgr;
    typedef std::map<std::string, HTexture, string_less_compare> NameIndex;

    HTextureMgr m_textures;
    NameIndex   m_textureNames;
};
