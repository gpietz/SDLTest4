#pragma once

#include "core/Types.h"

struct TimeHelper {
    static bool HasTimeExpired(SystemTimePoint& timePoint, const int millisDuration) noexcept;
};
