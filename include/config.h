/* Copyright 2024 Damian Kolaska */
#pragma once

#include <ncurses.h>
#include <stdint.h>

#include <string>

#include "input.h"

namespace ascii_combat {

constexpr uint32_t kWindowWidth = 80;
constexpr uint32_t kWindowHeight = 20;
constexpr uint32_t kWindowPadding = 2;
constexpr int32_t kWindowTopLeftX = 0;
constexpr int32_t kWindowTopLeftY = 0;

const char* kAssetsFileName = "assets.txt";

constexpr KeyMapping kKeyMap1{KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};
constexpr KeyMapping kKeyMap2{'w', 's', 'a', 'd'};

constexpr uint32_t kPlayerWidth = 3;
constexpr uint32_t kPlayerHeight = 4;
constexpr uint32_t kPlayerAttackRange = 3;
constexpr uint32_t kPlayerHealth = 3;

}  // namespace ascii_combat
