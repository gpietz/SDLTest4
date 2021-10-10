#include "SpriteSheet.h"
#include "parser/TextureAtlas.h"
#include "helpers/IoHelpers.h"
#include "helpers/TimeHelper.h"

SpriteSheet::SpriteSheet(u32 frameDuration) {
    initialize();
    m_frameDuration = frameDuration;
}

void SpriteSheet::initialize() noexcept {
    m_texture       = "";
    m_currentFrame  = 0;
    m_frameDuration = 1000;
    m_frameChanged  = SystemClock::now();
}

bool SpriteSheet::loadFromXmlFile(const char* xmlFileName, const RenderContext* renderContext) noexcept {
    // load texture atlas
    auto xmlFilePath = getFullResourcePath(xmlFileName);
    auto textureAtlas = TextureAtlas();
    if (!textureAtlas.loadFromFile(xmlFileName)) {
        return false;
    }

    // load texture from texture atlas file
    auto textureManager = renderContext->m_textureManager;
    auto renderer = renderContext->m_renderer;
    auto texture = textureManager->getTexture(textureAtlas.getImagePath().c_str(), renderer);
    m_texture = texture->getName();

    for (auto& it : textureAtlas.sprites) {
        auto sprite = it.get();
        auto frame = Frame {
            .x      = sprite->xPos,
            .y      = sprite->yPos,
            .width  = sprite->width,
            .height = sprite->height
        };
        m_frames.push_back(frame);
    }

    m_xmlFileName = xmlFileName;
    return true;
}

u32 SpriteSheet::getFrameDuration() const noexcept {
    return m_frameDuration;
}

void SpriteSheet::setFrameDuration(u32 frameDuration) noexcept {
    m_frameDuration = std::max<u32>(100, frameDuration);
}

SDL_Rect SpriteSheet::getRectAsValue() noexcept {
    updateFrame();
    auto currentFrame = m_frames[m_currentFrame];
    auto rect = SDL_Rect{
        .x = currentFrame.x,
        .y = currentFrame.y,
        .w = currentFrame.width,
        .h = currentFrame.height
    };
    return rect;
}

void SpriteSheet::getRect(SDL_Rect* rect) noexcept {
    updateFrame();
    auto currentFrame = m_frames[m_currentFrame];
    rect->x = currentFrame.x;
    rect->y = currentFrame.y;
    rect->w = currentFrame.width;
    rect->h = currentFrame.height;
}

std::string SpriteSheet::getTexture() const noexcept {
    return m_texture;
}

void SpriteSheet::updateFrame() noexcept {
    if (TimeHelper::HasTimeExpired(m_frameChanged, m_frameDuration)) {
        m_currentFrame++;
        if (m_currentFrame >= m_frames.size()) {
            m_currentFrame = 0;
        }
    }
}
