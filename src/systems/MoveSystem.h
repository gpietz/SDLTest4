#pragma once

#include "core/Types.h"
#include <entt/entity/fwd.hpp>
#include "RenderContext.h"

namespace MoveSystem {
    void update(entt::registry& registry, const RenderContext* renderContext, FDuration dt) noexcept;
};
