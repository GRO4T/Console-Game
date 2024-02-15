/* Copyright 2024 Damian Kolaska */
#pragma once

#include <chrono>  // NOLINT
#include <thread>  // NOLINT
#include <vector>

#include "assets.h"
#include "config.h"
#include "input.h"
#include "player.h"
#include "window.h"

namespace ascii_combat {

#define IS_CURR_ANIM_ATTACK_ANIM                                 \
    (player.GetCurrentClip() == &player.GetAnimations()[3][0] || \
     player.GetCurrentClip() == &player.GetAnimations()[3][1])

class Game {
   public:
    Game(Window& window, const std::vector<std::string>& map,  // NOLINT
         const std::vector<KeyMapping>& key_maps);
    void GameLoop();

   private:
    enum class State { kRunning, kPaused, kExit };
    enum class Answer { kYes, kNo };

    Window& window_;
    State state_;
    const std::vector<std::string>& map_;
    std::vector<KeyMapping> key_maps_;
    std::vector<Player> players_;

    Input GetInput();
    void Update(const Input& input);
    void Draw();
    void End();

    Answer AskYesOrNo(const std::string& question);
    static void DisplayTextCenter(Window& window, const std::string& text,  // NOLINT
                                  int32_t offset_y = 0);
    void DisplayUI();
};

}  // namespace ascii_combat
