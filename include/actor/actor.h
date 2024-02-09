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

extern std::vector<std::string> map;  // represents actual view of the map

extern WINDOW *win;

namespace ascii_combat {

class Player;

class Actor {
   public:
    int x, y;    // current position of the top-left corner
    int vx, vy;  // current velocity
    int health;
    bool onGround;
    bool faceRight;    // if character is facing right
    bool isAttacking;  // if character is performing an Attack
    bool isDead;

    // animation
    std::vector<std::vector<Clip>> animations;
    Clip *current_clip_;

    Actor() {}
    Actor(int _x, int _y, int _vx, int _vy, int _range, int _health,
          std::vector<std::vector<Clip>> &anims);

    void Set(int _x, int _y, int _vx, int _vy, int _range, int _health,
             std::vector<std::vector<Clip>> &anims);

    void Draw();
    void Update(int _vx, int _vy);

    void GotHit() { isDead = --health <= 0; }

    int GetRange() const { return range; }
    int GetWidth() const { return w; }

   protected:
    // attributes
    int range;  // attack range
    int w, h;   // creature size
};

}  // namespace ascii_combat
