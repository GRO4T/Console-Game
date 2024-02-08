#pragma once

#include <vector>

#include "creature.h"

namespace ascii_combat {

using PlayerAnimations = std::vector<std::vector<Clip>>;
using Map = std::vector<std::string>;

class Assets {
   public:
    Assets() = default;
    void Load(const std::string& assets_filename);

    const PlayerAnimations& GetPlayerAnimations() const;
    const std::vector<Map>& GetMaps() const;

   private:
    // TODO: Refactor PlayerAnimations.
    PlayerAnimations player_animations_;
    std::vector<Map> maps_;
};

}  // namespace ascii_combat