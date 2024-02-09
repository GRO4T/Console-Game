#include "actor/player.h"

namespace ascii_combat {

void Player::Attack(Player &opponent) {
    if (inRange(this, &opponent)) {
        opponent.GotHit();
        if (opponent.isDead) {
        } else {
            if (x > opponent.x)
                opponent.Update(-1, -1);
            else
                opponent.Update(1, -1);
        }
    }
}

bool Player::inRange(Actor *player, Actor *target) {
    if ((player->faceRight && player->x < target->x) ||
        (!player->faceRight && player->x > target->x)) {  // if pointing towards enemy
        if (abs(target->y - player->y) <= 1 &&
            abs(target->x - player->x) <= player->GetRange() + player->GetWidth())
            return true;
    }
    return false;
}

}  // namespace ascii_combat
