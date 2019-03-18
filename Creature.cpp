#include"Creature.hpp"
#define ANCHOR_X 1
#define ANCHOR_Y 1

extern vector<string> map; //represents actual view of the map

extern WINDOW * win;

void Clip::Draw(Creature &creature, int y, int x){
    int i = creature.h - frames[state].size();
    int q = 0;
    for (auto r : frames[state]){
        q = 0;
        for (auto c : r){
            if (c != ' ')
                mvwprintw(win, ANCHOR_Y + y + i, ANCHOR_X + x + q, "%c", c);
            ++q;
        }
        ++i;
    }
    --timer;
    if (timer <= 0){
        timer = delay;
        state = (state + 1) % len;
    }
}
Creature::Creature(int _x, int _y, int _vx, int _vy, int _range, int _health, vector<vector<Clip>> &anims)
{
    Set(_x, _y, _vx, _vy, _range, _health, anims);
}

void Creature::Set(int _x, int _y, int _vx, int _vy, int _range, int _health, vector<vector<Clip>> &anims)
{
    x = _x; y = _y; vx = _vx; vy = _vy; range = _range;
    health = _health;
    faceRight = true;
    isAttacking = false;
    for (auto anim : anims){
        animations.emplace_back(anim);
    }
    currentClip = &animations[0][0];

    w = 4;
    h = 3;
    Update(0, 0);
    //if (y + h < BOUND_UP)
    //  if (map[y + h][x] == 'M')
    //     onGround = true;
}

void Creature::Draw(){
    int drawX = x;
    if (isAttacking){
        if (faceRight)
            currentClip = &animations[3][0];
        else{
            currentClip = &animations[3][1];
            drawX -= 4;
        }

        if (currentClip->state == currentClip->len - 1){
            isAttacking = false;
            currentClip->timer = 1;
        }
    }
    else if (onGround && vx != 0){
        //if is walking
        //if facing right
        if (faceRight)
            currentClip = &animations[1][0];
        else
            currentClip = &animations[1][1];
    }
    else if (!onGround)
        currentClip = &animations[2][0]; //falling
    else
        currentClip = &animations[0][0]; //idle

    currentClip->Draw(*this, y, drawX);
}

void Creature::Update(int _vx, int _vy){
    if (vy == 0 && _vy != 0 && onGround){
        vy = _vy;
        onGround = false;
    }
    else if(!onGround){
        ++vy;
    }

    if (_vx != 0 && onGround){ //slower on ground
        if (_vx > 0)
            --_vx;
        else if (_vx < 0)
            ++_vx;
    }
    vx = _vx;

    //collision detection
    bool cOccured = false;
    if (vy > 0){
        int VY = vy;
        for (int v = 1; v <= VY && !cOccured; ++v){ //start with vector with length 1
            vy = v;
            if (y + h - 1 + v < BOUND_UP){ //range checking
                for (int c = x + 1; c < x + w - 1 && !cOccured; ++c){ //check if we can move by this vector
                    if (map[y + h + v - 1][c] == 'M'){ //if not, answer is vector 1 shorter otherwise we continue
                        vy = v - 1;
                        onGround = true;
                        cOccured = true;
                    }
                }
            }
            else{
                vy = v - 1;
                cOccured = true;
            }
        }
    }
    else if (vy < 0){
        int VY = vy;
        for (int v = -1; v >= VY && !cOccured; --v){
            vy = v;
            if (y + v >= 0){
                for (int c = x + 1; c < x + w - 1 && !cOccured; ++c){
                    if (map[y + v][c] == 'M'){
                        vy = v + 1;
                        cOccured = true;
                    }
                }
            }
            else{
                vy = v + 1;
                cOccured = true;
            }
        }
    }
    else{
        if (map[y + h][x + 1] == ' ' && map[y + h][x + 2] == ' ')
            onGround = false;
    }
    // for x Axis it will be little more complicated
    // because we wile be taking into account shapes
    cOccured = false;
    if (vx > 0){
        int VX = vx;
        for (int v = 1; v <= VX && !cOccured; ++v){
            vx = v;
            if (x + w + v - 1 < BOUND_RIGHT){
                for (int c = x + w - 1; c >= x && !cOccured; --c){
                    for (int r = y; r < y + h; ++r){
                        if (map[r][c + v] == 'M' && animations[0][0].frames[0][r - y][c - x] != ' '){
                            vx = v - 1;
                            cOccured = true;
                        }
                    }
                }
            }
            else{
                vx = v - 1;
                cOccured = true;
            }
        }
    }
    else if (vx < 0){
        int VX = vx;
        for (int v = -1; v >= VX && !cOccured; --v){
            vx = v;
            if (x + v >= 0){
                for (int c = x; c < x + w && !cOccured; ++c){
                    for (int r = y; r < y + h; ++r){
                        if (map[r][c + v] == 'M' && animations[0][0].frames[0][r - y][c - x] != ' '){
                            vx = v + 1;
                            cOccured = true;
                        }
                    }
                }
            }
            else{
                vx = v + 1;
                cOccured = true;
            }
        }
    }
    vy = CLAMP(vy, -3, 3);

    x += vx;
    y += vy;

    x = CLAMP(x, 0, BOUND_RIGHT - w);
    y = CLAMP(y, 0, BOUND_UP - h - 1);
}

/*bool compare(Enemy & enemy1, Enemy & enemy2){
    if (abs(enemy1.x - player.x) <= abs(enemy2.x - player.x))
        return true;
    return false;
}*/

int inRange(Player &player, Creature *target){
    if ((player.faceRight && player.x < target->x) || (!player.faceRight && player.x > target->x)){//if pointing towards enemy
        if (abs(target->y - player.y) <= 1 && abs(target->x - player.x) <= player.range + player.w)
            return 1;
    }
    return 0;
}

void Player::Attack(list<Enemy *> &enemies){
    isAttacking = true;
    for (std::list<Enemy *>::iterator it=enemies.begin(); it!=enemies.end(); ++it){
        if (inRange(*this, *it)){
            if ((*it)->IsDead()){
                delete *it;
                it = enemies.erase(it);
            }
            else{
                if (x > (*it)->x)
                    (*it)->Update(-1, -1);
                else
                    (*it)->Update(1, -1);
            }
        }
    }
}

void Player::Attack(Player &opponent){
    if (inRange(*this, &opponent)){
        if (opponent.IsDead()){

        }
        else{
            if (x > opponent.x)
                opponent.Update(-1, -1);
            else
                opponent.Update(1, -1);
        }
    }
}

