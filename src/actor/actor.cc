#include "actor/actor.h"

#include "config.h"

namespace ascii_combat {

Actor::Actor(int _x, int _y, int _vx, int _vy, int _range, int _health,
             std::vector<std::vector<Clip>> &anims) {
    Set(_x, _y, _vx, _vy, _range, _health, anims);
}

void Actor::Set(int _x, int _y, int _vx, int _vy, int _range, int _health,
                std::vector<std::vector<Clip>> &anims) {
    x = _x;
    y = _y;
    vx = _vx;
    vy = _vy;
    range = _range;
    health = _health;
    faceRight = true;
    isAttacking = false;
    isDead = false;

    assert(anims.size() == ANIM_NUM);
    for (auto anim : anims) {
        animations.emplace_back(anim);
    }
    current_clip_ = &animations[0][0];

    w = 4;
    h = 3;
    Update(0, 0);
}

void Actor::Draw() {
    int drawX = x;

    if (isDead) {
        current_clip_ = &animations[4][0];
    } else if (isAttacking) {
        if (faceRight)
            current_clip_ = &animations[3][0];
        else {
            current_clip_ = &animations[3][1];
            drawX -= 4;
        }

        if (current_clip_->IsFinished()) {
            isAttacking = false;
            current_clip_->SetTimer(1);
        }
    } else if (onGround && vx != 0) {
        // if is walking
        // if facing right
        if (faceRight)
            current_clip_ = &animations[1][0];
        else
            current_clip_ = &animations[1][1];
    } else if (!onGround)
        current_clip_ = &animations[2][0];  // falling
    else
        current_clip_ = &animations[0][0];  // idle

    current_clip_->Draw(win, y, drawX);
}

void Actor::Update(int _vx, int _vy) {
    if (!isDead) {
        if (vy == 0 && _vy != 0 && onGround) {
            vy = _vy;
            onGround = false;
        } else if (!onGround) {
            ++vy;
        }

        if (_vx != 0 && onGround) {  // slower on ground
            if (_vx > 0)
                --_vx;
            else if (_vx < 0)
                ++_vx;
        }
        vx = _vx;

        // collision detection
        bool cOccured = false;
        if (vy > 0) {
            int VY = vy;
            for (int v = 1; v <= VY && !cOccured; ++v) {  // start with vector with length 1
                vy = v;
                if (y + h - 1 + v < kWindowHeight) {  // range checking
                    for (int c = x + 1; c < x + w - 1 && !cOccured;
                         ++c) {                              // check if we can move by this vector
                        if (map[y + h + v - 1][c] == 'M') {  // if not, answer is vector 1 shorter
                                                             // otherwise we continue
                            vy = v - 1;
                            onGround = true;
                            cOccured = true;
                        }
                    }
                } else {
                    vy = v - 1;
                    cOccured = true;
                }
            }
        } else if (vy < 0) {
            int VY = vy;
            for (int v = -1; v >= VY && !cOccured; --v) {
                vy = v;
                if (y + v >= 0) {
                    for (int c = x + 1; c < x + w - 1 && !cOccured; ++c) {
                        if (map[y + v][c] == 'M') {
                            vy = v + 1;
                            cOccured = true;
                        }
                    }
                } else {
                    vy = v + 1;
                    cOccured = true;
                }
            }
        } else {
            if (map[y + h][x + 1] == ' ' && map[y + h][x + 2] == ' ') onGround = false;
        }
        // for x Axis it will be little more complicated
        // because we wile be taking into account shapes
        cOccured = false;
        if (vx > 0) {
            int VX = vx;
            for (int v = 1; v <= VX && !cOccured; ++v) {
                vx = v;
                if (x + w + v - 1 < kWindowWidth) {
                    for (int c = x + w - 1; c >= x && !cOccured; --c) {
                        for (int r = y; r < y + h; ++r) {
                            if (map[r][c + v] == 'M' &&
                                animations[0][0].GetFrames()[0][r - y][c - x] != ' ') {
                                vx = v - 1;
                                cOccured = true;
                            }
                        }
                    }
                } else {
                    vx = v - 1;
                    cOccured = true;
                }
            }
        } else if (vx < 0) {
            int VX = vx;
            for (int v = -1; v >= VX && !cOccured; --v) {
                vx = v;
                if (x + v >= 0) {
                    for (int c = x; c < x + w && !cOccured; ++c) {
                        for (int r = y; r < y + h; ++r) {
                            if (map[r][c + v] == 'M' &&
                                animations[0][0].GetFrames()[0][r - y][c - x] != ' ') {
                                vx = v + 1;
                                cOccured = true;
                            }
                        }
                    }
                } else {
                    vx = v + 1;
                    cOccured = true;
                }
            }
        }
        vy = CLAMP(vy, -3, 3);

        x += vx;
        y += vy;

        x = CLAMP(x, 0, (int32_t)kWindowWidth - w);
        y = CLAMP(y, 0, (int32_t)kWindowHeight - h - 1);
    }
}

}  // namespace ascii_combat
