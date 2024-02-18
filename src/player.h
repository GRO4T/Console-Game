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
#include "utils.h"
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

    struct Attributes {
        uint32_t speed;
        uint32_t jump_force;
        uint32_t attack_range;
        uint32_t health;
    };

    enum class Direction { kRight, kLeft };

    Player(const Box &box, const Attributes &attrs, const Direction &facing_direction,
           const Controls &controls, const Map &map);

    int32_t GetX() const;
    int32_t GetY() const;
    uint32_t GetWidth() const;
    int32_t GetHealth() const;

    const Direction &GetFacingDirection() const;
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

    const uint32_t width_;
    const uint32_t height_;
    const int32_t speed_;
    const int32_t jump_force_;
    const uint32_t attack_range_;

    int32_t health_;
    Direction facing_direction_;
    bool is_grounded_;
    bool is_attacking_;
    bool is_dead_;

    const Controls controls_;
    const Map map_;

    std::vector<std::vector<Clip>> animations_;
    Clip *current_clip_;

    std::pair<int32_t, int32_t> UpdateMovement(const Input &input);
    void UpdateCurrentClip();
};

class PlayerFactory {
   public:
    static Player CreatePlayer(int32_t x, int32_t y, const Player::Direction &facing_direction,
                               const Player::Controls &controls, const Map &map);
};

}  // namespace ascii_combat
