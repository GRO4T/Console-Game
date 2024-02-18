/* Copyright 2024 Damian Kolaska */
#pragma once

#include <string>
#include <vector>

#include "clip.h"

namespace ascii_combat {

using PlayerAnimations = std::vector<std::vector<Clip>>;
using Map = std::vector<std::string>;

class Assets {
   public:
    Assets(const Assets& other) = delete;
    void operator=(const Assets&) = delete;

    static Assets& Instance();

    PlayerAnimations& GetPlayerAnimations();
    const std::vector<Map>& GetMaps() const;

   private:
    Assets();
    ~Assets() = default;

    // TODO(GRO4T): Refactor PlayerAnimations.
    PlayerAnimations player_animations_;
    std::vector<Map> maps_;
};

}  // namespace ascii_combat
