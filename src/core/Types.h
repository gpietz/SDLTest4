#pragma once

#include <memory>
#include <chrono>
#include <optional>

typedef unsigned int  u32;
typedef unsigned long u64;

struct nop {
    template<typename T>
    void operator() (T const &) const noexcept { }
};

template<typename T>
using nop_unique_ptr = std::unique_ptr<T, nop>;

using FDuration         = std::chrono::duration<float>;
using SystemClock       = std::chrono::system_clock;
using SystemTimePoint   = std::chrono::time_point<SystemClock>;
using OptionalTimePoint = std::optional<SystemTimePoint>;
using HighResClock      = std::chrono::high_resolution_clock;
using Millis            = std::chrono::milliseconds;
using Secs              = std::chrono::seconds;
