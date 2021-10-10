#include "TimeHelper.h"
#include <chrono>

bool TimeHelper::HasTimeExpired(SystemTimePoint& timePoint, const int millisDuration) noexcept {
    auto duration = SystemClock::now() - timePoint;
    auto duration_ms = std::chrono::duration_cast<Millis>(duration).count();
    if (duration_ms >= millisDuration) {
        timePoint = SystemClock::now();
        return true;
    }
    return false;
}
