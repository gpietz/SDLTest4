#pragma once

#include <entt/locator/locator.hpp>
#include "TextureManager.h"

struct Locator {
    using Textures = entt::service_locator<TextureManager>;
};
