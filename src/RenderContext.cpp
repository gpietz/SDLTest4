#include "RenderContext.h"

RenderContext::RenderContext(SDL_Renderer *renderer, TextureManager *textureManager)
    : m_renderer(renderer), m_textureManager(textureManager) {
    validate();
}

void RenderContext::validate() const {
    assert(m_renderer != nullptr);
    assert(m_textureManager != nullptr);
}
