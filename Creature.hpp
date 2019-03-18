#ifndef CREATURE_H
#define CREATURE_H

#include<list>
#include<vector>
#include<ncurses.h>
#include<iostream>
#include<math.h>

#define CLAMP(x, lower, upper) (min(upper, max(x, lower)))
#define BOUND_UP 20
#define BOUND_RIGHT 80

using namespace std;
class Creature;
class Clip{
    public:
    vector<vector<string>> frames;
    int len; //length of the clip
    int state; //currently played animations
    int timer; //how much time until frame should be changed
    int delay; //initial timer value

    void Draw(Creature &creature, int y, int x);
    Clip(int _timer) { len = 0; state = 0; timer = _timer; delay = _timer; }
    ~Clip(){
        len = 0;
    }
};

class Creature{
    public:
//attributes
        int x, y; //current position of the top-left corner
        int vx, vy; //current velocity
        int range; //attack range
        int w, h; //creature size
        int health;
//animation
        vector<vector<Clip>> animations;
        Clip * currentClip;
//flags and timers
        bool onGround;
        bool faceRight; //if character is facing right
        bool isAttacking; //if character is performing an Attack

        virtual ~Creature() {}

        void SetClip(vector<Clip> &anim, int i);
        Creature(){}
        Creature(int _x, int _y, int _vx, int _vy, int _range, int _health, vector<vector<Clip>> &anims);

        void Set(int _x, int _y, int _vx, int _vy, int _range, int _health, vector<vector<Clip>> &anims);

        void Draw();
        void Update(int _vx, int _vy);

        bool IsDead(){ return (--health <= 0); }
};
//currently useless
class Enemy : public Creature{
    public:
        Enemy() {}
        Enemy(int _x, int _y, int _vx, int _vy, int _range, int _health, vector<vector<Clip>> &anims)
            : Creature(_x, _y, _vx, _vy, _range, _health, anims){}
        void Attack(Creature player){}
};

class Player : public Creature{
    public:
        Player(int _x, int _y, int _vx, int _vy, int _range, int _health, vector<vector<Clip>> &anims)
            : Creature(_x, _y, _vx, _vy, _range, _health, anims){}
        Player(){}
        ~Player(){}
        void Attack(list<Enemy *> &enemies);
        void Attack(Player &opponent);

    private:
       // int weapon;
       // int ammo;
};

bool compare(Enemy & enemy1, Enemy & enemy2); //currently not used, may be useful when optimising Player::Attack

#endif


