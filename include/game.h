#ifndef GAME_H
#define GAME_H

#if !defined(NDEBUG)
#define DEBUG 1
#else
#define DEBUG 0
#endif

#include<chrono>
#include"creature.h"
#include<thread>

#define KEY_NUM 4
#define IS_CURR_ANIM_ATTACK_ANIM (player.currentClip == &player.animations[3][0] || player.currentClip == &player.animations[3][1])

using namespace std::chrono_literals;

extern WINDOW * win;
extern vector<vector<Clip>> playerAnimations;
extern vector<string> map;

struct Key{
    int value;
    bool pressed;
};

//in Windows we have 'kbhit()' which does almost exactly the same
inline int KeyBoardHit(struct timeval tv){
    fd_set rfds;

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    return select(1, &rfds, NULL, NULL, &tv);
}


//last module let's try to write it correctly in terms of C++ convention
class Game{
    public:
        // static std::chrono::time_point<std::chrono::system_clock> tp1;
        //static std::chrono::time_point<std::chrono::system_clock> tp2;
        virtual void ProcessInput() = 0;
        virtual void Update() = 0;
        virtual void Draw() = 0;

        virtual void DisplayUI() = 0;

        virtual void GameLoop() = 0;

        Game(){ PAUSE = false; EXIT = false; }

    protected:
        std::chrono::duration<float> elapsedTime;
        float fElapsedTime;
        bool PAUSE;
        bool EXIT;
};

class SinglePlayer : Game{

};

void HandlePlayer(Player &player, Key * keys, int n, Player &opponent);

class Multiplayer : public Game{
    public:
        Multiplayer() : Game() {
            player1.Set(0, 0, 0, 0, 3, 3, playerAnimations);
            player2.Set(76, 0, 0, 0, 3, 3, playerAnimations);
        }

        void GameLoop();

    private:
        Player player1;
        Player player2;

        Key keys1[KEY_NUM] = { {KEY_DOWN, 0}, {KEY_UP, 0}, {KEY_LEFT, 0}, {KEY_RIGHT, 0} }; //ARROWS
        Key keys2[KEY_NUM] = { {'s', 0}, {'w', 0}, {'a', 0}, {'d', 0} }; //WSAD

        void ProcessInput();
        void Update(){
            HandlePlayer(player1, keys1, KEY_NUM, player2);
            HandlePlayer(player2, keys2, KEY_NUM, player1);
        }
        void DisplayUI(){
            string health_bar2 = "health = " + to_string(player2.health >= 0 ? player2.health : 0);
            wattron(win, A_REVERSE);
            mvwprintw(win, 1, 1, "Player1");
            mvwprintw(win, 1, BOUND_RIGHT - 7, "Player2");
            wattroff(win, A_REVERSE);
            mvwprintw(win, 2, BOUND_RIGHT - health_bar2.length(), health_bar2.c_str());
            mvwprintw(win, 2, 1, "health = %d", player1.health >= 0 ? player1.health : 0);

        }
        void Draw(){
            wclear(win);
            box(win, 0, 0);
            int i = 0;
            for (auto r : map){
                mvwprintw(win, 1 + i++, 1, "%s", r.c_str());
            }
            if (player2.isDead){
                player2.Draw();
                player1.Draw();
            }
            else{
                player1.Draw();
                player2.Draw();
            }
            DisplayUI();
        }
};
#endif
