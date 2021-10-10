#pragma once

#include "core/Types.h"
#include "TextureManager.h"
#include <entt/entity/fwd.hpp>
#include <iostream>
#include <random>
#include "RenderContext.h"
#include "SpriteSheet.h"

class SpawnerSystem {
public:
    SpawnerSystem();

    void update(entt::registry &registry, FDuration dt) noexcept;
    void set(TextureManager *textureManager) noexcept;
    void set(RenderContext *renderContext) noexcept;
    void set(SpriteSheet *spriteSheet) noexcept;

private:
    TextureManager *m_textures = nullptr;
    RenderContext *m_renderContext = nullptr;
    SpriteSheet *m_redHatRun = nullptr;
    bool m_poweredSpriteSpawned = false;
    OptionalTimePoint m_bjarneSpawned = std::nullopt;
    OptionalTimePoint m_lastRocketSpawned = std::nullopt;
    OptionalTimePoint m_lastSpawnCyclesIncreased = std::nullopt;
    SystemTimePoint m_lastRedHatRunSeen;
    int m_lastSpawnOffset = 0;
    int m_lastSpawnSpeed = 0;
    int m_spawnCycles = 1;
    std::random_device m_rd;

    int m_minXOffset = 0;

    void spawnRocket(entt::registry &registry) noexcept;
    void spawnRedHat(entt::registry& registry) noexcept;
    int getRandom(int min, int max, bool negative = false) noexcept;
    int getRocketSpeed() noexcept;
    int getRocketYOffset(int canvasHeight) noexcept;
};
