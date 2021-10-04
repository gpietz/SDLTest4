#pragma once

#include <string>
#include <memory>
#include <cassert>
#include <SDL2/SDL.h>
#include "RenderContext.h"

class DrawCommand {
public:
    virtual ~DrawCommand() = default;;
    virtual void render(const RenderContext* renderContext) noexcept { };
};

namespace DrawCommands {
    /////////////////////////////////////////////////////////////////////////////
    // ClearScreen
    /////////////////////////////////////////////////////////////////////////////
    class ClearScreen : public DrawCommand {
    public:
        ClearScreen();
        ClearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

        void render(const RenderContext* renderContext) noexcept override;

        // Properties
        std::unique_ptr<SDL_Color> m_color;
    };

    /////////////////////////////////////////////////////////////////////////////
    // DrawImage
    /////////////////////////////////////////////////////////////////////////////
    class DrawImage : public DrawCommand {
    public:
        explicit DrawImage(const char* texture, SDL_Point* position = nullptr, float rotate = 0);

        void render(const RenderContext* renderContext) noexcept override;

        // Properties
        std::string m_texture;
        std::unique_ptr<SDL_Point> m_position;
        float m_rotation;
    };
};
