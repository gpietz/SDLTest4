#include "SpawnerSystem.h"
#include "Components.h"
#include "TextureManager.h"
#include <entt/entity/registry.hpp>
#include <iostream>
#include "helpers/TimeHelper.h"

const int SPAWN_DELAY = 1500;
const int SPAWN_CYCLE_INCREASE_DELAY = 3000;
const int SPAWN_PADDING = 30;
const int SPAWN_MIN_DISTANCE = 40;
const int ROCKET_MAX_SPEED = 8;
const int ROCKET_WIDTH = 129;

float getDuration(SystemTimePoint timePoint) {
    auto duration = SystemClock::now() - timePoint;
    return (float) std::chrono::duration_cast<Millis>(duration).count();
}

SpawnerSystem::SpawnerSystem() {
    std::mt19937 gen(m_rd());
}

void SpawnerSystem::update(entt::registry& registry, FDuration dt) noexcept {
    if (!m_poweredSpriteSpawned) {
        m_poweredSpriteSpawned = true;
        auto spriteEntity = registry.create();
        registry.emplace<Renderable>(spriteEntity, Renderable { .textureName = "resources/gfx/powered.png" });
        registry.emplace<FadeInOut>(spriteEntity,
                                    FadeInOut { .durationIn = 1200, .durationOut = 1200, .durationPeak = 1800 });
        return;
    }

    if (registry.empty<Renderable>() && m_bjarneSpawned == std::nullopt) {
        m_bjarneSpawned = SystemClock::now();
        auto imageEntity = registry.create();
        registry.emplace<Renderable>(imageEntity, Renderable { .textureName = "resources/gfx/bjarne.jpg" });
        registry.emplace<FadeInOut>(imageEntity, FadeInOut { .durationIn = 2000, .keepWhenDone = true });
        return;
    }

    if (m_bjarneSpawned.has_value()) {
        spawnRocket(registry);
        spawnRedHat(registry);
    }
}

void SpawnerSystem::set(TextureManager* textureManager) noexcept {
    m_textures = textureManager;
}

void SpawnerSystem::set(RenderContext* renderContext) noexcept {
    m_renderContext = renderContext;
}

void SpawnerSystem::set(SpriteSheet* spriteSheet) noexcept {
    m_redHatRun = spriteSheet;
}

void SpawnerSystem::spawnRocket(entt::registry& registry) noexcept {
    if (m_lastRocketSpawned.has_value() && getDuration(m_lastRocketSpawned.value()) < SPAWN_DELAY)
        return;

    if (!m_lastSpawnCyclesIncreased.has_value()) {
        m_lastSpawnCyclesIncreased = SystemClock::now();
    } else if (getDuration(m_lastSpawnCyclesIncreased.value()) > SPAWN_CYCLE_INCREASE_DELAY) {
        m_lastSpawnCyclesIncreased = SystemClock ::now();
        m_spawnCycles++;
    }

    int canvasWidth = 0, canvasHeight = 0;
    SDL_GetRendererOutputSize(m_renderContext->m_renderer, &canvasWidth, &canvasHeight);

    for (int i = 0; i < m_spawnCycles; ++i) {
        auto rocketSpeed = getRocketSpeed();
        auto xOffset = rocketSpeed > 0
            ? getRandom(ROCKET_WIDTH * 2, canvasWidth / 2, true)
            : getRandom(canvasWidth + ROCKET_WIDTH, canvasWidth + canvasWidth / 2, false);
        auto yOffset = getRocketYOffset(canvasHeight);
        auto rocketEntity = registry.create();
        registry.emplace<Renderable>(rocketEntity, Renderable { .layer = 200, .textureName = "resources/gfx/f1.png" });
        registry.emplace<Rocket>(rocketEntity, Rocket { .speed = rocketSpeed });
        registry.emplace<Position>(rocketEntity, Position { .x = xOffset, .y = yOffset });

        m_lastRocketSpawned = SystemClock::now();
        m_lastSpawnOffset = yOffset;
        m_lastSpawnSpeed = rocketSpeed;
    }
}

void SpawnerSystem::spawnRedHat(entt::registry& registry) noexcept {
    auto redHatCount = registry.view<RedHatRun>().size();
    if (redHatCount == 0 && TimeHelper::HasTimeExpired(m_lastRedHatRunSeen, 3500)) {
        int canvasWidth = 0, canvasHeight = 0;
        SDL_GetRendererOutputSize(m_renderContext->m_renderer, &canvasWidth, &canvasHeight);

        auto redHatRect = m_redHatRun->getRectAsValue();
        auto redHatEntity = registry.create();
        registry.emplace<Renderable>(redHatEntity, Renderable {.layer = 300, .spriteSheet = m_redHatRun });
        registry.emplace<Position>(redHatEntity, Position { .x = -(redHatRect.w), .y = canvasHeight - redHatRect.h - 10 });
        registry.emplace<RedHatRun>(redHatEntity, RedHatRun { .width = static_cast<u32>(redHatRect.w) });
    }
    if (redHatCount > 0) {
        m_lastRedHatRunSeen = SystemClock::now();
    }
}

int SpawnerSystem::getRandom(int min, int max, bool negative) noexcept {
    std::uniform_int_distribution<> distribution(min, max);
    auto randomValue = distribution(m_rd);
    return negative ? -(randomValue) : randomValue;
}

int SpawnerSystem::getRocketSpeed() noexcept {
    int rocketSpeed = getRandom(-ROCKET_MAX_SPEED, ROCKET_MAX_SPEED);
    while ((m_lastSpawnSpeed < 0 && rocketSpeed < 0) &&
           (m_lastSpawnSpeed > 0 && rocketSpeed > 0)) {
        rocketSpeed = getRandom(-ROCKET_MAX_SPEED, ROCKET_MAX_SPEED);
    }
    return rocketSpeed;
}

int SpawnerSystem::getRocketYOffset(int canvasHeight) noexcept {
    int yOffset = 0;
    const int halfHeight = canvasHeight / 2;
    while (true) {
        yOffset  = getRandom(SPAWN_PADDING, canvasHeight - (SPAWN_PADDING * 2));
        auto distance = abs(m_lastSpawnOffset - yOffset);
        auto sameArea = (m_lastSpawnOffset < halfHeight && yOffset < halfHeight);
        if ((m_lastSpawnOffset == 0 || distance > SPAWN_MIN_DISTANCE) &&
            yOffset > SPAWN_PADDING &&
            yOffset < (canvasHeight - SPAWN_PADDING) &&
            !sameArea)
            break;
    }
    return yOffset;
}
