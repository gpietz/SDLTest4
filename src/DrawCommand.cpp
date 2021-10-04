#include <cassert>
#include <SDL2/SDL.h>
#include "DrawCommand.h"

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

        if (m_position == nullptr) {
            // if no position is given in the DrawImage command the texture will be centered
            int target_width = 0, target_height = 0;
            SDL_GetRendererOutputSize(renderContext->m_renderer, &target_width, &target_height);
            target_rect.x = (target_width/2 - texture->getWidth()/2);
            target_rect.y = (target_height/2 - texture->getHeight()/2);
        } else {
            target_rect.x = m_position->x;
            target_rect.y = m_position->y;
        }

        SDL_RenderCopyEx(renderContext->m_renderer, texture->getSdlTexture(), nullptr, &target_rect,
                         m_rotation, nullptr, SDL_FLIP_NONE);
    }
}
