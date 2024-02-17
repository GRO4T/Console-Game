/* Copyright 2024 Damian Kolaska */
#pragma once

#include <sys/select.h>

#include <unordered_map>

namespace ascii_combat {

enum class Key { kW, kS, kA, kD, kUp, kDown, kLeft, kRight, kQ };

enum class KeyState { kPressed, kReleased };

using Input = std::unordered_map<Key, KeyState>;

}  // namespace ascii_combat
