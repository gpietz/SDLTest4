#pragma once

typedef unsigned int  u32;
typedef unsigned long u64;

struct nop {
    template<typename T>
    void operator() (T const &) const noexcept { }
};

template<typename T>
using nop_unique_ptr = std::unique_ptr<T, nop>;
