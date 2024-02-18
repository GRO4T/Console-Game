/* Copyright 2024 Damian Kolaska */
#include <string.h>

#include <algorithm>
#include <fstream>
#include <list>
#include <vector>

#include "assets.h"
#include "config.h"
#include "game.h"
#include "menu.h"
#include "window.h"

// TODO(GRO4T): This should be read from the assets file.
const std::vector<std::string> kGameModes = {"(WIP) Player vs AI", "Local Multiplayer",
                                             "(WIP) Multiplayer", "Exit"};
const std::vector<std::string> kMapNames = {"Plain Flat", "Hills", "Valley", "Platformer"};

using namespace ascii_combat;  // NOLINT

int main() {
    Window window(kWindowHeight, kWindowWidth, kWindowTopLeftX, kWindowTopLeftY, kWindowPadding);

    std::string game_mode = "";
    do {
        game_mode = Menu(window, kGameModes).GetChoice();

        if ("Local Multiplayer" == game_mode) {
            std::string map_name = Menu(window, kMapNames).GetChoice();
            std::size_t map_id =
                std::find(kMapNames.begin(), kMapNames.end(), map_name) - kMapNames.begin();
            auto map = Assets::Instance().GetMaps()[map_id];
            Game game(window, map);
            game.Run();
        }
    } while ("Exit" != game_mode);

    return 0;
}
