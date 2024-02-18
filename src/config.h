/* Copyright 2024 Damian Kolaska */
#pragma once

#include <stdint.h>

#include "input.h"
#include "player.h"

namespace ascii_combat {

constexpr uint32_t kWindowWidth = 80;
constexpr uint32_t kWindowHeight = 20;
constexpr uint32_t kWindowPadding = 2;
constexpr int32_t kWindowTopLeftX = 0;
constexpr int32_t kWindowTopLeftY = 0;

const char* const kAssetsFileName = "assets.txt";

const char* const kPlayerVsAIMode = "(WIP) Player vs AI";
const char* const kLocalMultiplayerMode = "Local Multiplayer";
const char* const kMultiplayerMode = "(WIP) Multiplayer";
const char* const kExit = "Exit";

constexpr Player::Controls kControlsPlayer1{Key::kW, Key::kS, Key::kA, Key::kD};
constexpr Player::Controls kControlsPlayer2{Key::kUp, Key::kDown, Key::kLeft, Key::kRight};

constexpr uint32_t kPlayerWidth = 3;
constexpr uint32_t kPlayerHeight = 4;
constexpr uint32_t kPlayerSpeed = 3;
constexpr uint32_t kPlayerJumpForce = 3;
constexpr uint32_t kPlayerAttackRange = 3;
constexpr uint32_t kPlayerHealth = 3;

}  // namespace ascii_combat
