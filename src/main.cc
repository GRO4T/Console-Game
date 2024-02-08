#include <string.h>

#include <algorithm>
#include <fstream>
#include <list>

#include "assets.h"
#include "game.h"
#include "menu.h"

constexpr int kWindowTopLeftX = 0;
constexpr int kWindowTopLeftY = 0;

const std::string kAssetsFileName = "assets.txt";
// TODO: This should be read from the assets file.
const std::vector<std::string> kGameModes = {"SINGLEPLAYER", "MULTIPLAYER", "EXIT"};
const std::vector<std::string> kMapNames = {"PLAIN FLAT", "HILLS", "VALLEY", "PLATFORMER"};

std::vector<std::vector<Clip>> player_animations;
vector<vector<string>> maps;
vector<string> map;

WINDOW* win;

int main() {
    initscr();
    noecho();
    cbreak();     // stop line buffering, pass everything
    curs_set(0);  // make cursor invisible

    win = newwin(BOUND_UP + 2, BOUND_RIGHT + 2, kWindowTopLeftX,
                 kWindowTopLeftY);  //+2 is for the box
    keypad(win, TRUE);              // enable special characters

    ascii_combat::Assets assets;
    assets.Load(kAssetsFileName);

    // NOTE: workaround for now
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
