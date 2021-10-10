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
            return ::stricmp(left.c_str(), right.c_str()) < 0;
        }
    };

    typedef HandleManager<Texture, HTexture> HTextureMgr;
    typedef std::map<std::string, HTexture, string_less_compare> NameIndex;

    HTextureMgr m_textures;
    NameIndex   m_textureNames;
};

typedef std::shared_ptr<TextureManager> SharedTextureManager;
