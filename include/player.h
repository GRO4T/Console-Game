/* Copyright 2024 Damian Kolaska */
#pragma once

#include <math.h>

#include <cassert>
#include <iostream>
#include <list>
#include <utility>
#include <vector>

#include "assets.h"
#include "clip.h"
#include "input.h"
#include "window.h"

namespace ascii_combat {

class Player {
   public:
    struct Controls {
        Key jump;
        Key attack;
        Key left;
        Key right;
    };

    enum class Direction { kRight, kLeft };

    Player(int x, int y, int width, int height, int speed, int jump_force, int attack_range,
           int health, bool is_facing_right, const Controls &controls, const Map &map,
           std::vector<std::vector<Clip>> &anims);

    int32_t GetX() const;
    int32_t GetY() const;
    uint32_t GetWidth() const;
    int32_t GetHealth() const;

    bool IsFacingRight() const;
    bool IsDead() const;

    uint32_t GetAttackRange() const;

    const std::vector<std::vector<Clip>> &GetAnimations() const;
    Clip *GetCurrentClip() const;

    void Draw(Window &window);
    void Update(const Input &input, Player &opponent);
    void Move(int32_t dx, int32_t dy);

    void GotHit(uint32_t damage, const Direction &attacked_from);

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

    const Controls controls_;
    const Map map_;

    std::vector<std::vector<Clip>> animations_;
    Clip *current_clip_;

    std::pair<int32_t, int32_t> UpdateMovement(const Input &input);
    void UpdateCurrentClip();
    void Attack(Player &opponent);
    bool IsOpponentInAttackRange(Player &opponent);
};

class PlayerFactory {
   public:
    static Player CreatePlayer(int x, int y, bool is_facing_right, const Player::Controls &controls,
                               const Map &map);
};

}  // namespace ascii_combat
