/* Copyright 2024 Damian Kolaska */
#pragma once

#include <chrono>  // NOLINT
#include <string>
#include <vector>

#include "assets.h"
#include "config.h"
#include "event.h"
#include "input.h"
#include "player.h"
#include "window.h"

namespace ascii_combat {

#define IS_CURR_ANIM_ATTACK_ANIM                                 \
    (player.GetCurrentClip() == &player.GetAnimations()[3][0] || \
     player.GetCurrentClip() == &player.GetAnimations()[3][1])

class Game {
   public:
    Game(Window& window, const std::vector<std::string>& map);
    void Run();

   private:
    enum class State { kRunning, kPaused, kExit };
    enum class Answer { kYes, kNo };

    Window& window_;
    State state_;
    const std::vector<std::string>& map_;
    std::vector<Player> players_;

    Input GetInput();
    void ProcessEvents();
    void Update(const Input& input);
    void Draw();
    void End();

    void Handle(const PlayerAttackEvent& event);

    Answer AskYesOrNo(const std::string& question);
    void DisplayUI();
};

}  // namespace ascii_combat
