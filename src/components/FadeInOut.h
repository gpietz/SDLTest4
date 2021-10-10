#pragma once

#include "core/Types.h"

struct FadeInOut {
    float durationIn   = 0;
    float durationOut  = 0;
    float durationPeak = 0;
    bool  keepWhenDone = false;

    // Receives the time at which the fade-in effect has been started.
    OptionalTimePoint startTime = std::nullopt;

    // Returns the total time this fading process will take.
    [[nodiscard]] float getTotalDuration() const {
        return durationIn + durationPeak + durationOut;
    }
};
