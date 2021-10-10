#pragma once

#include <SDL2/SDL.h>
#include "RenderContext.h"

namespace DrawHelper {
    void completeTargetRect(const RenderContext* renderContext,
                            Texture* texture,
                            SDL_Rect* targetRect,
                            SDL_Point* targetPosition) noexcept;

    bool drawImage(const RenderContext* renderContext,
                   Texture* texture,
                   SDL_Rect* srcRect,
                   SDL_Rect* targetRect,
                   float rotation,
                   SDL_Point* center,
                   SDL_RendererFlip flipMode,
                   float opacity = 1.0) noexcept;
};
