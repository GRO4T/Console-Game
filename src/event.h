/* Copyright 2024 Damian Kolaska */
#pragma once

#include <stdint.h>

#include "player.h"

namespace ascii_combat {

struct Event {
    virtual ~Event() = default;
};

struct PlayerAttackEvent : public Event {
   public:
    PlayerAttackEvent(const Player& attacker, Player& target, const uint32_t damage)
        : attacker(attacker), target(target), damage(damage) {}
    const Player& attacker;
    Player& target;
    const uint32_t damage;
};

}  // namespace ascii_combat
