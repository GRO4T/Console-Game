/* Copyright 2024 Damian Kolaska */
#pragma once

#include <sys/select.h>

#include <unordered_map>

namespace ascii_combat {

using KeyCode = int;

struct KeyMapping {
    int up;
    int down;
    int left;
    int right;
};

enum class KeyState { kPressed, kReleased };

bool WaitForInput(timeval timeout);

using Input = std::unordered_map<KeyCode, KeyState>;

}  // namespace ascii_combat
