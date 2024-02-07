#include <string.h>

#include <fstream>
#include <list>

#include "game.h"
#include "menu.h"

#define WINX 0
#define WINY 0

#define FILENAME "assets.txt"

// some defines for if-statements

using namespace std;

// Assets
vector<vector<Clip>> playerAnimations;

vector<vector<string>> maps;
vector<string> map;

void GetAssets(string filename);  // gets assets and initializes characters

WINDOW* win;

int main() {
    initscr();
    noecho();
    cbreak();     // stop line buffering, pass everything
    curs_set(0);  // make cursor invisible

    win =
        newwin(BOUND_UP + 2, BOUND_RIGHT + 2, WINX, WINY);  //+2 is for the box
    keypad(win, TRUE);  // enable special characters

    GetAssets(FILENAME);

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

void GetAssets(string filename) {
    ifstream file;
    file.open(filename);

    string row = "";
    do {
        getline(file, row);
        if (row != "EOF") {
            if (row == "Player") {
                for (int i = 0; i < ANIM_NUM; ++i) {
                    vector<Clip> anim;
                    while (1) {
                        getline(file, row);
                        if (row == "EOA")  // End Of Animation
                            break;
                        else {
                            Clip clip(stoi(row));
                            while (1) {
                                vector<string> frame;
                                getline(file, row);
                                if (row == "EOC")  // End Of Clip
                                    break;
                                else {
                                    frame.emplace_back(row);
                                    while (1) {
                                        getline(file, row);
                                        if (row == "EFM")  // End Of Frame
                                            break;
                                        else {
                                            frame.emplace_back(row);
                                        }
                                    }
                                    clip.frames.emplace_back(frame);
                                    ++(clip.len);
                                }
                            }
                            anim.emplace_back(clip);
                        }
                    }
                    playerAnimations.emplace_back(anim);
                }
            } else if (row == "Map") {
                vector<string> map;
                while (1) {
                    getline(file, row);
                    if (row == "EOM")
                        break;
                    else
                        map.emplace_back(row);
                }
                maps.emplace_back(map);
            }
        }

    } while (row != "EOF");
    file.close();
}
