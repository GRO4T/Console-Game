#include "actor/player.h"

namespace ascii_combat {

void Player::Attack(Player &opponent) {
    if (IsInRange(this, &opponent)) {
        opponent.GotHit();
        if (opponent.IsDead()) {
        } else {
            if (GetX() > opponent.GetX())
                opponent.Update(-1, -1);
            else
                opponent.Update(1, -1);
        }
    }
}

bool Player::IsInRange(Actor *player, Actor *target) {
    if ((player->IsFacingRight() && player->GetX() < target->GetX()) ||
        (!player->IsFacingRight() &&
         player->GetX() > target->GetX())) {  // if pointing towards enemy
        if (abs(target->GetY() - player->GetY()) <= 1 &&
            abs(target->GetX() - player->GetX()) <= player->GetAttackRange() + player->GetWidth())
            return true;
    }
    return false;
}

}  // namespace ascii_combat
