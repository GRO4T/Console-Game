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
const std::vector<std::string> kGameModes = {"SINGLEPLAYER", "MULTIPLAYER", "EXIT"};
const std::vector<std::string> kMapNames = {"PLAIN FLAT", "HILLS", "VALLEY", "PLATFORMER"};

using namespace ascii_combat;  // NOLINT

int main() {
    Window window(kWindowHeight, kWindowWidth, kWindowTopLeftX, kWindowTopLeftY, kWindowPadding);

    std::string game_mode = "";
    do {
        game_mode = Menu(window.GetHandle(), kGameModes).GetChoice();

        if ("MULTIPLAYER" == game_mode) {
            std::string map_name = Menu(window.GetHandle(), kMapNames).GetChoice();
            std::size_t map_id =
                std::find(kMapNames.begin(), kMapNames.end(), map_name) - kMapNames.begin();
            auto map = Assets::Instance().GetMaps()[map_id];
            Game game(window, map, {kKeyMap1, kKeyMap2});
            game.GameLoop();
        }
    } while ("EXIT" != game_mode);

    endwin();
}
