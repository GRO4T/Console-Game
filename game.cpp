#include"game.hpp"

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
                    if (!player.isAttacking){
                        vx += -vSpeed;
                        player.faceRight = false;
                        BREAK = true; break;
                    }
                case 3:
                    if (!player.isAttacking){
                        vx += vSpeed;
                        player.faceRight = true;
                        BREAK = true; break;
                    }
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
bool Ask(string msg){
    mvwprintw(win, BOUND_UP / 2, (BOUND_RIGHT - msg.length()) / 2, msg.c_str());
    int c;
    while(1){
        c = wgetch(win);
        if (c == 'y' || c == 'Y'){
            return true;
        }
        else if (c == 'n' || c == 'N'){
            return false;
        }
    }
}
void Multiplayer::GameLoop(){
    auto tp1 = std::chrono::system_clock::now();
    auto tp2 = tp1;
    while(1){
        tp1 = std::chrono::system_clock::now();
        ProcessInput();
        if (!PAUSE)
            Update();
        Draw();
        if (PAUSE){
            string msg = "Do you really want to quit? (Y/N)";
            PAUSE = EXIT = Ask(msg);
        }

        tp2 = std::chrono::system_clock::now(); elapsedTime = tp2 - tp1; tp1 = tp2;
        fElapsedTime = elapsedTime.count();
        this_thread::sleep_for(66.8ms - elapsedTime);

        //debugging
        /*
           if (DEBUG){
           mvwprintw(win, 1,1,"%d %d onGround = %d attack = %d", player1.x, player1.y, player1.onGround, player1.isAttacking);
           mvwprintw(win, 5, 3, "%d %d %d", player1.vx, player1.vy, 'g');
           mvwprintw(win, 3, 3, "%d %d %d %d", keys1[0].pressed, keys1[1].pressed, keys1[2].pressed, keys1[3].pressed);
           mvwprintw(win, 4, 3, "%d %d %d %d", keys2[0].pressed, keys2[1].pressed, keys2[2].pressed, keys2[3].pressed);
           }
           */
        if (player1.isDead || player2.isDead){
            string msg1;
            string msg2 = "Press any key to continue...";

            if (player1.isDead && player2.isDead)
                msg1 = "It's a draw!";
            else if (player1.isDead)
                msg1 = "Player2 won!";
            else if (player2.isDead)
                msg1 = "Player1 won!";

            mvwprintw(win, BOUND_UP / 2, (BOUND_RIGHT - msg1.length()) / 2, msg1.c_str());
            mvwprintw(win, (BOUND_UP / 2) + 1, (BOUND_RIGHT - msg2.length()) / 2, msg2.c_str());

            wgetch(win);
            EXIT = true;
        }

        if (EXIT)
            break;
        wrefresh(win);
    }
}

void Multiplayer::ProcessInput(){
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
                    PAUSE = true;
            }
        }
    }
}
