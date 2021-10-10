#pragma once

#include <vector>
#include <string>
#include "core/Types.h"
#include "Sprite.h"
#include "RenderContext.h"

class SpriteSheet {
public:
    SpriteSheet(u32 frameDuration = 100);

    bool loadFromXmlFile(const char* xmlFileName, const RenderContext* renderContext) noexcept;

    struct Frame {
        int x;
        int y;
        int width;
        int height;
    };

    u32 getFrameDuration() const noexcept;
    void setFrameDuration(u32 frameDuration) noexcept;

    SDL_Rect getRectAsValue() noexcept;
    void getRect(SDL_Rect* rect) noexcept;

    std::string getTexture() const noexcept;

private:
    std::string m_texture;
    u32 m_currentFrame;
    u32 m_frameDuration;
    SystemTimePoint m_frameChanged;
    std::vector<Frame> m_frames;
    std::string m_xmlFileName;

    void initialize() noexcept;
    void updateFrame() noexcept;
};
