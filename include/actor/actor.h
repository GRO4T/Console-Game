#pragma once

#include <math.h>
#include <ncurses.h>

#include <cassert>
#include <iostream>
#include <list>
#include <vector>

#include "clip.h"

#define CLAMP(x, lower, upper) (std::min(upper, std::max(x, lower)))

#define ANIM_NUM 5

extern std::vector<std::string> map;  // TODO: Move to Game singleton.

extern WINDOW *win;  // TODO: Draw should be called by the game engine.

namespace ascii_combat {

class Player;

class Actor {
   public:
    Actor() {}
    Actor(int _x, int _y, int _vx, int _vy, int _range, int _health,
          std::vector<std::vector<Clip>> &anims);

    void Set(int _x, int _y, int _vx, int _vy, int _range, int _health,
             const std::vector<std::vector<Clip>> &anims);

    int32_t GetX() const;
    int32_t GetY() const;
    int32_t GetVelocityX() const;
    int32_t GetVelocityY() const;
    uint32_t GetWidth() const;

    int32_t GetHealth() const;
    bool IsGrounded() const;
    bool IsFacingRight() const;
    bool IsAttacking() const;
    bool IsDead() const;

    uint32_t GetAttackRange() const;

    const std::vector<std::vector<Clip>>& GetAnimations() const;
    Clip* GetCurrentClip() const;

    void SetIsFacingRight(bool value);
    void SetIsAttacking(bool value);

    void Draw();
    void Update(int _vx, int _vy);

    void GotHit() { is_dead_ = --health_ <= 0; }

   protected:
    int32_t x_;  // NOTE: position is calculated from top-left corner.
    int32_t y_;
    uint32_t width_;
    uint32_t height_;
    int32_t vx_;
    int32_t vy_;

    int32_t health_;
    bool is_grounded_;
    bool is_facing_right_;
    bool is_attacking_;
    bool is_dead_;

    uint32_t attack_range_;

    std::vector<std::vector<Clip>> animations_;
    Clip *current_clip_;
};

}  // namespace ascii_combat
