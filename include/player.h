/* Copyright 2024 Damian Kolaska */
#pragma once

#include <math.h>
#include <ncurses.h>

#include <cassert>
#include <iostream>
#include <list>
#include <utility>
#include <vector>

#include "assets.h"
#include "clip.h"
#include "input.h"

namespace ascii_combat {

class Player {
   public:
    Player(int x, int y, int width, int height, int speed, int jump_force, int attack_range,
           int health, bool is_facing_right, const KeyMapping &key_map, const Map &map,
           std::vector<std::vector<Clip>> &anims);  // NOLINT

    int32_t GetHealth() const;
    bool IsDead() const;

    const std::vector<std::vector<Clip>> &GetAnimations() const;
    Clip *GetCurrentClip() const;

    void Draw(WINDOW *win);
    void Update(const Input &input, Player &opponent);  // NOLINT
    void Move(int32_t dx, int32_t dy);

   private:
    int32_t x_;  // NOTE: position is calculated from top-left corner.
    int32_t y_;
    int32_t vx_;
    int32_t vy_;
    uint32_t width_;
    uint32_t height_;
    int32_t speed_;
    int32_t jump_force_;

    int32_t health_;
    bool is_grounded_;
    bool is_facing_right_;
    bool is_attacking_;
    bool is_dead_;

    uint32_t attack_range_;

    const KeyMapping key_map_;
    const Map map_;

    std::vector<std::vector<Clip>> animations_;
    Clip *current_clip_;

    std::pair<int32_t, int32_t> UpdateMovement(const Input &input);
    void UpdateCurrentClip();
    void Attack(Player &opponent);  // NOLINT
    void GotHit() { is_dead_ = --health_ <= 0; }
    bool IsOpponentInAttackRange(Player &opponent);  // NOLINT
};

class PlayerFactory {
   public:
    static Player CreatePlayer(int x, int y, bool is_facing_right, const KeyMapping &key_map,
                               const Map &map);
};

}  // namespace ascii_combat
