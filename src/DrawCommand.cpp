#include <SDL2/SDL.h>
#include <cmath>
#include "core/Assert.h"
#include "DrawCommand.h"
#include "DrawHelper.h"

namespace DrawCommands {
    /////////////////////////////////////////////////////////////////////////////
    // ClearScreen
    /////////////////////////////////////////////////////////////////////////////

    ClearScreen::ClearScreen() = default;

    ClearScreen::ClearScreen(Uint8 r, Uint8 g, Uint8 b,  Uint8 a) {
        m_color = std::make_unique<SDL_Color>();
        m_color->r = r;
        m_color->g = g;
        m_color->b = b;
        m_color->a = a;
    }

    void ClearScreen::render(const RenderContext* renderContext) noexcept {
        assert(renderContext != nullptr);
        if (m_color != nullptr) {
            auto color = m_color.get();
            SDL_SetRenderDrawColor(renderContext->m_renderer, color->r, color->g, color->b, color->a);
        }
        SDL_RenderClear(renderContext->m_renderer);
    }

    /////////////////////////////////////////////////////////////////////////////
    // DrawImage
    /////////////////////////////////////////////////////////////////////////////

    DrawImage::DrawImage(const char* texture, SDL_Point* position, float rotation)
    {
        m_texture  = texture;
        m_position = std::unique_ptr<SDL_Point>(position);
        m_rotation = rotation;
    }

    void DrawImage::render(const RenderContext* renderContext) noexcept {
        assert(renderContext != nullptr);

        auto texture_mgr = renderContext->m_textureManager;
        auto texture     = texture_mgr->getTexture(m_texture.c_str());
        auto target_rect = SdlHelpers::createRect(0, 0, texture->getWidth(), texture->getHeight());

        DrawHelper::completeTargetRect(renderContext, texture, &target_rect, m_position.get());
        DrawHelper::drawImage(renderContext, texture, nullptr, &target_rect, m_rotation, nullptr, SDL_FLIP_NONE);
    }

    /////////////////////////////////////////////////////////////////////////////
    // DrawSprite
    /////////////////////////////////////////////////////////////////////////////

    DrawSprite::DrawSprite(const Sprite *sprite) : sprite(sprite) {
    }

    void DrawSprite::render(const RenderContext* renderContext) noexcept {
        assert(renderContext != nullptr);

        auto texture = sprite->getTexture();
        auto targetRect = SdlHelpers::createRect(0, 0, texture->getWidth(), texture->getHeight());

        DrawHelper::completeTargetRect(renderContext, texture, &targetRect, position.get());
        DrawHelper::drawImage(renderContext, texture, nullptr, &targetRect, rotation, nullptr, sprite->flipMode, opacity);
    }

    /////////////////////////////////////////////////////////////////////////////
    // DrawSpriteSheet
    /////////////////////////////////////////////////////////////////////////////
    DrawSpriteSheet::DrawSpriteSheet(const SpriteSheet* spriteSheet) : spriteSheet(spriteSheet) {
    }

    void DrawSpriteSheet::render(const RenderContext* renderContext) noexcept {
        assert(renderContext != nullptr);

        auto sourceRect = const_cast<SpriteSheet*>(spriteSheet)->getRectAsValue();
        auto targetRect = SdlHelpers::createRect(0, 0, sourceRect.w, sourceRect.h);

        if (position != nullptr) {
            targetRect.x = position->x;
            targetRect.y = position->y;
        }

        auto texture = renderContext->m_textureManager->getTexture(spriteSheet->getTexture().c_str());
        DrawHelper::drawImage(renderContext, texture, &sourceRect, &targetRect, 0, nullptr, SDL_FLIP_NONE);
    }
}
