/* Copyright 2024 Damian Kolaska */
#pragma once

#include <stdint.h>

namespace ascii_combat {

struct Box {
    int32_t x;
    int32_t y;
    uint32_t width;
    uint32_t height;
};

template <typename T>
struct Vector2D {
    T x;
    T y;
};

}  // namespace ascii_combat
