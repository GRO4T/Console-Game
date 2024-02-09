#pragma once

#include <vector>

#include "actor/actor.h"

namespace ascii_combat {

using PlayerAnimations = std::vector<std::vector<Clip>>;
using Map = std::vector<std::string>;

class Assets {
   public:
    Assets(Assets& other) = delete;
    void operator=(const Assets&) = delete;

    static Assets& Instance();

    const PlayerAnimations& GetPlayerAnimations() const;
    const std::vector<Map>& GetMaps() const;

   private:
    Assets();
    ~Assets() = default;

    // TODO: Refactor PlayerAnimations.
    PlayerAnimations player_animations_;
    std::vector<Map> maps_;
};

}  // namespace ascii_combat