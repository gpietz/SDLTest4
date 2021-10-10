#pragma once

#include "RenderContext.h"
#include <entt/entity/fwd.hpp>

namespace RenderSystem {
    void draw(const RenderContext* renderContext, entt::registry& registry);
};
