#ifndef GAME_H
#define GAME_H

#if !defined(NDEBUG)
#pragma message ("DEBUG MODE")
#define DEBUG 1
#else
#pragma message ("RELEASE MODE")
#define DEBUG 0
#endif

#include<chrono>
#include"Creature.hpp"
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
int KeyBoardHit(struct timeval tv){
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
        std::chrono::duration<float> elapsedTime;
        float fElapsedTime;
        bool EXIT;

        virtual void ProcessInput(){}
        virtual void Update(){}
        virtual void Draw(){}

        virtual void GameLoop(){}

        Game(){ EXIT = false; }



};

class SinglePlayer : Game{

};

void HandlePlayer(Player &player, Key * keys, int n, Player &opponent){
    int vx, vy;
    vx = vy = 0;
    int vSpeed = 2;
    int jumpForce = 3;

    bool BREAK = false;
    for (int i = 0; i < KEY_NUM && !BREAK; ++i){
        if (keys[i].pressed){
            switch(i){
                case 0:
                    if (player.onGround)
                        player.isAttacking = true;
                    BREAK = true; break;
                case 1:
                    vy += -jumpForce;
                    BREAK = true; break;
                case 2:
                    vx += -vSpeed;
                    player.faceRight = false;
                    BREAK = true; break;
                case 3:
                    vx += vSpeed;
                    player.faceRight = true;
                    BREAK = true; break;
            }
        }
    }

    if (!(player.isAttacking)){
        player.Update(vx, vy);
    }
    else{
        if (IS_CURR_ANIM_ATTACK_ANIM){
            //if attack animation is at its final stage (to avoid hitting with a sword at the beginning of the movement)
            if (player.currentClip->state == player.currentClip->len - 1){
                player.Attack(opponent);
            }
        }
    }
}

class Multiplayer : Game{
    public:
        Player player1;
        Player player2;

        Key keys1[KEY_NUM] = { {KEY_DOWN, 0}, {KEY_UP, 0}, {KEY_LEFT, 0}, {KEY_RIGHT, 0} }; //ARROWS
        Key keys2[KEY_NUM] = { {115, 0}, {'w', 0}, {'a', 0}, {'d', 0} }; //WSAD

        Multiplayer() : Game() {
            player1.Set(0, 16, 0, 0, 3, 3, playerAnimations);
            player2.Set(40, 16, 0, 0, 3, 3, playerAnimations);
        }
        void ProcessInput(){
            int n = KEY_NUM;
            int c;
            for (int i = 0; i < n; ++i){
                keys1[i].pressed = false;
                keys2[i].pressed = false;
            }

            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 1;
            int x = 0; //we must limit keys that can be written
            while (KeyBoardHit(tv)) {
                c = wgetch(win);
                if (x++ < n){
                    for (int i = 0; i < n; ++i){
                        if (c == keys1[i].value)
                            keys1[i].pressed = true;
                        else if (c == keys2[i].value)
                            keys2[i].pressed = true;
                        else if (c == 'q')
                            EXIT = true;
                    }
                }
            }
        }
        void GameLoop(){
            auto tp1 = std::chrono::system_clock::now();
            auto tp2 = tp1;
            while(1){
                tp1 = std::chrono::system_clock::now();
                ProcessInput();
                Update();
                Draw();

                tp2 = std::chrono::system_clock::now(); elapsedTime = tp2 - tp1; tp1 = tp2;
                fElapsedTime = elapsedTime.count();
                this_thread::sleep_for(66.8ms - elapsedTime);

                if (DEBUG){

                    mvwprintw(win, 1,1,"%d %d onGround = %d attack = %d", player1.x, player1.y, player1.onGround, player1.isAttacking);
                    mvwprintw(win, 5, 3, "%d %d %d", player1.vx, player1.vy, 'g');
                }

                if (EXIT)
                    break;
                wrefresh(win);
            }
        }
        void Draw(){
            wclear(win);
            box(win, 0, 0);
            int i = 0;
            for (auto r : map){
                mvwprintw(win, 1 + i++, 1, "%s", r.c_str());
            }
            player1.Draw();
            player2.Draw();
            mvwprintw(win, 3, 3, "%d %d %d %d", keys1[0].pressed, keys1[1].pressed, keys1[2].pressed, keys1[3].pressed);
            mvwprintw(win, 4, 3, "%d %d %d %d", keys2[0].value, keys2[1].value, keys2[2].pressed, keys2[3].pressed);
        }

        void Update(){
            HandlePlayer(player1, keys1, KEY_NUM, player2);
            HandlePlayer(player2, keys2, KEY_NUM, player1);
        }
    private:


};
#endif
