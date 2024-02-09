#pragma once

#include <vector>

#include "actor.h"

namespace ascii_combat {

class Player : public Actor {
   public:
    Player() {}
    Player(int _x, int _y, int _vx, int _vy, int _range, int _health,
           std::vector<std::vector<Clip>> &anims)
        : Actor(_x, _y, _vx, _vy, _range, _health, anims) {}

    void Attack(Player &opponent);

   private:
    bool IsInRange(Actor *player, Actor *target);
};

}  // namespace ascii_combat
