#include "MoveSystem.h"
#include "Components.h"
#include <entt/entt.hpp>

const int ROCKET_WIDTH = 129;

void moveRedHatRun(entt::registry& registry, const RenderContext* renderContext, int canvasWidth);

void MoveSystem::update(entt::registry& registry, const RenderContext* renderContext, FDuration dt) noexcept {
    int canvasWidth = 0, canvasHeight = 0;
    SDL_GetRendererOutputSize(renderContext->m_renderer, &canvasWidth, &canvasHeight);

    auto view = registry.view<const Renderable, Position, Rocket>();
    for (auto [entity, rd, pos, rocket] : view.each()) {
        if ((rocket.speed > 0 && pos.x > canvasWidth + ROCKET_WIDTH) ||
            (rocket.speed < 0 && pos.x < -ROCKET_WIDTH)) {
            registry.destroy(entity);
        }
        pos.x += rocket.speed;
    }

    moveRedHatRun(registry, renderContext, canvasWidth);
}

void moveRedHatRun(entt::registry& registry, const RenderContext* renderContext, int canvasWidth) {
    auto view = registry.view<const Renderable, Position, RedHatRun>();
    for (auto [entity, rd, pos, redHatRun] : view.each()) {
        pos.x += 6;
        if (pos.x > (int)(canvasWidth + redHatRun.width)) {
            registry.destroy(entity);
        }
    }
}
