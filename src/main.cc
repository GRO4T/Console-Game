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

// TODO: This should be read from the assets file.
const std::vector<std::string> kGameModes = {"SINGLEPLAYER", "MULTIPLAYER", "EXIT"};
const std::vector<std::string> kMapNames = {"PLAIN FLAT", "HILLS", "VALLEY", "PLATFORMER"};

std::vector<std::vector<ascii_combat::Clip>> player_animations;
std::vector<std::vector<std::string>> maps;
std::vector<std::string> map;

WINDOW* win;

int main() {
    ascii_combat::Window window(kWindowHeight, kWindowWidth, kWindowTopLeftX, kWindowTopLeftY,
                                kWindowPadding);

    ascii_combat::Assets assets;
    assets.Load(kAssetsFileName);

    // NOTE: workaround for now
    win = window.GetHandle();
    player_animations = assets.GetPlayerAnimations();
    maps = assets.GetMaps();

    std::string game_mode = "";
    do {
        game_mode = ascii_combat::Menu(win, kGameModes).GetChoice();

        if ("MULTIPLAYER" == game_mode) {
            std::string map_name = ascii_combat::Menu(win, kMapNames).GetChoice();
            std::size_t map_id =
                std::find(kMapNames.begin(), kMapNames.end(), map_name) - kMapNames.begin();
            map = assets.GetMaps()[map_id];
            Multiplayer game;
            game.GameLoop();
        }
    } while ("EXIT" != game_mode);

    endwin();
}
