#pragma once

#include <SDL_render.h>
#include "TextureManager.h"

struct RenderContext {
    RenderContext(SDL_Renderer *renderer, TextureManager *textureManager);
    void validate() const;

    // Properties
    SDL_Renderer* m_renderer;
    TextureManager* m_textureManager;
};
