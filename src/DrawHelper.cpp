#include "DrawHelper.h"
#include "core/Assert.h"

void DrawHelper::completeTargetRect(const RenderContext* renderContext,
                                    Texture* texture,
                                    SDL_Rect* targetRect,
                                    SDL_Point* targetPosition) noexcept {
    M_Assert(renderContext != nullptr, "Reference to RenderContext object required!");
    M_Assert(texture != nullptr, "Reference to texture object required!");
    M_Assert(targetRect != nullptr, "Reference to target rectangle object required!");

    if (targetPosition == nullptr) {
        // if no position is given in the DrawImage command the texture will be centered
        int target_width = 0, target_height = 0;
        SDL_GetRendererOutputSize(renderContext->m_renderer, &target_width, &target_height);
        targetRect->x = (int) (target_width / 2 - texture->getWidth() / 2);
        targetRect->y = (int) (target_height / 2 - texture->getHeight() / 2);
    } else {
        targetRect->x = targetPosition->x;
        targetRect->y = targetPosition->y;
    }
}

bool DrawHelper::drawImage(const RenderContext* renderContext,
                           Texture* texture,
                           SDL_Rect* srcRect,
                           SDL_Rect* targetRect,
                           float rotation,
                           SDL_Point* center,
                           SDL_RendererFlip flipMode,
                           float opacity) noexcept {
    M_Assert(renderContext != nullptr, "Reference to render context object required!");
    M_Assert(texture != nullptr,  "Reference to texture required!");

    auto sdl_texture = texture->getSdlTexture();
    if (sdl_texture == nullptr) {
        return false;
    }

    if (opacity < 1.0) {
        auto alpha = (int) std::round(255 * opacity);
        SDL_SetTextureAlphaMod(sdl_texture, alpha);
    }

    SDL_RenderCopyEx(renderContext->m_renderer, sdl_texture, srcRect, targetRect, rotation, center, flipMode);

    if (opacity < 1.0) {
        SDL_SetTextureAlphaMod(sdl_texture, 255);
    }

    return true;
}
