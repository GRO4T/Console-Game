#include <string.h>

#include <fstream>
#include <list>

#include "assets.h"
#include "game.h"
#include "menu.h"

constexpr int kWindowTopLeftX = 0;
constexpr int kWindowTopLeftY = 0;

const std::string kAssetsFileName = "assets.txt";

std::vector<std::vector<Clip>> player_animations;
vector<vector<string>> maps;
vector<string> map;

void GetAssets(string filename);  // gets assets and initializes characters

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

    int choice;
    do {
        vector<string> choices = {"SINGLEPLAYER", "MULTIPLAYER", "EXIT"};
        choice = Menu(choices, 3);
        if (choice == 1) {
        } else if (choice == 2) {
            vector<string> choices = {"PLAIN FLAT", "HILLS", "VALLEY",
                                      "PLATFORMER"};
            int c = Menu(choices, 4);
            int mapN = c == 0 ? 3 : c - 1;
            map = maps[mapN];
            Multiplayer game;
            game.GameLoop();
        }

    } while (choice);  // menu returns 0 when exit is chosen

    endwin();
}
