#ifndef CREATURE_H
#define CREATURE_H

#include <math.h>
#include <ncurses.h>

#include <cassert>
#include <iostream>
#include <list>
#include <vector>

#include "clip.h"

using namespace ascii_combat;

#define CLAMP(x, lower, upper) (min(upper, max(x, lower)))
#define BOUND_UP 20
#define BOUND_RIGHT 80

#define ANIM_NUM 5

using namespace std;

extern vector<string> map;  // represents actual view of the map

extern WINDOW *win;

class Creature;
class Player;
class Enemy;

class Creature {
   public:
    int x, y;    // current position of the top-left corner
    int vx, vy;  // current velocity
    int health;
    bool onGround;
    bool faceRight;    // if character is facing right
    bool isAttacking;  // if character is performing an Attack
    bool isDead;

    // animation
    vector<vector<Clip>> animations;
    Clip *current_clip_;

    friend int inRange(Player &player, Creature *target);

    Creature() {}
    Creature(int _x, int _y, int _vx, int _vy, int _range, int _health,
             vector<vector<Clip>> &anims);

    void Set(int _x, int _y, int _vx, int _vy, int _range, int _health,
             vector<vector<Clip>> &anims);

    void Draw();
    void Update(int _vx, int _vy);

    void GotHit() { isDead = --health <= 0; }

   protected:
    // attributes
    int range;  // attack range
    int w, h;   // creature size
};

class Player : public Creature {
   public:
    Player() {}
    Player(int _x, int _y, int _vx, int _vy, int _range, int _health, vector<vector<Clip>> &anims)
        : Creature(_x, _y, _vx, _vy, _range, _health, anims) {}

    void Attack(list<Enemy *> &enemies);
    void Attack(Player &opponent);

   private:
    // int weapon;
    // int ammo;
};

// currently useless
class Enemy : public Creature {
   public:
    Enemy() {}
    Enemy(int _x, int _y, int _vx, int _vy, int _range, int _health, vector<vector<Clip>> &anims)
        : Creature(_x, _y, _vx, _vy, _range, _health, anims) {}
    void Attack(Player player) {}
};

// bool compare(Enemy & enemy1, Enemy & enemy2); //currently not used, may be
// useful when optimising Player::Attack

#endif
