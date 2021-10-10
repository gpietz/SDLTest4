#pragma once

#include <string>
#include <memory>
#include <cassert>
#include <SDL2/SDL.h>
#include "RenderContext.h"
#include "Sprite.h"
#include "SpriteSheet.h"

struct DrawCommand {
    virtual void render(const RenderContext* renderContext) noexcept { };
};

namespace DrawCommands {
    /////////////////////////////////////////////////////////////////////////////
    // ClearScreen
    /////////////////////////////////////////////////////////////////////////////
    struct ClearScreen : DrawCommand {
        ClearScreen();
        ClearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

        void render(const RenderContext* renderContext) noexcept override;

        // Properties
        std::unique_ptr<SDL_Color> m_color;
    };

    /////////////////////////////////////////////////////////////////////////////
    // DrawImage
    /////////////////////////////////////////////////////////////////////////////
    struct DrawImage : DrawCommand {
        explicit DrawImage(const char* texture, SDL_Point* position = nullptr, float rotate = 0);

        void render(const RenderContext* renderContext) noexcept override;

        // Properties
        std::string m_texture;
        std::unique_ptr<SDL_Point> m_position;
        float m_rotation;
    };

    /////////////////////////////////////////////////////////////////////////////
    // DrawSprite
    /////////////////////////////////////////////////////////////////////////////
    struct DrawSprite : DrawCommand {
        explicit DrawSprite(const Sprite* sprite);

        void render(const RenderContext* renderContext) noexcept override;

        // Properties
        const Sprite* sprite;
        std::unique_ptr<SDL_Point> position;
        float rotation{};
        float opacity = 1.0;
    };

    /////////////////////////////////////////////////////////////////////////////
    // DrawSpriteSheet
    /////////////////////////////////////////////////////////////////////////////
    struct DrawSpriteSheet : DrawCommand {
        DrawSpriteSheet(const SpriteSheet* spriteSheet);

        void render(const RenderContext* renderContext) noexcept override;

        // Properties
        const SpriteSheet* spriteSheet;
        std::unique_ptr<SDL_Point> position;
    };
};
