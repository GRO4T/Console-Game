#include "actor/actor.h"

#include "config.h"

namespace ascii_combat {

Actor::Actor(int _x, int _y, int _vx, int _vy, int _range, int _health,
             std::vector<std::vector<Clip>> &anims) {
    Set(_x, _y, _vx, _vy, _range, _health, anims);
}

void Actor::Set(int _x, int _y, int _vx, int _vy, int _range, int _health,
                const std::vector<std::vector<Clip>> &anims) {
    x_ = _x;
    y_ = _y;
    vx_ = _vx;
    vy_ = _vy;
    attack_range_ = _range;
    health_ = _health;
    is_facing_right_ = true;
    is_attacking_ = false;
    is_dead_ = false;

    assert(anims.size() == ANIM_NUM);
    for (auto anim : anims) {
        animations_.emplace_back(anim);
    }
    current_clip_ = &animations_[0][0];

    width_ = 4;
    height_ = 3;
    Update(0, 0);
}

int32_t Actor::GetX() const { return x_; }

int32_t Actor::GetY() const { return y_; }

int32_t Actor::GetVelocityX() const { return vx_; }

int32_t Actor::GetVelocityY() const { return vy_; }

uint32_t Actor::GetAttackRange() const { return attack_range_; }

uint32_t Actor::GetWidth() const { return width_; }

int32_t Actor::GetHealth() const { return health_; }

const std::vector<std::vector<Clip>>& Actor::GetAnimations() const {
    return animations_;
}

Clip* Actor::GetCurrentClip() const { return current_clip_; }

bool Actor::IsGrounded() const { return is_grounded_; }

bool Actor::IsFacingRight() const { return is_facing_right_; }

bool Actor::IsAttacking() const { return is_attacking_; }

bool Actor::IsDead() const { return is_dead_; }

void Actor::SetIsFacingRight(bool value) { is_facing_right_ = value; }

void Actor::SetIsAttacking(bool value) { is_attacking_ = value; }

void Actor::Draw() {
    int draw_x = GetX();

    if (IsDead()) {
        current_clip_ = &animations_[4][0];
    } else if (is_attacking_) {
        if (is_facing_right_)
            current_clip_ = &animations_[3][0];
        else {
            current_clip_ = &animations_[3][1];
            draw_x -= 4;
        }

        if (current_clip_->IsFinished()) {
            is_attacking_ = false;
            current_clip_->SetTimer(1);
        }
    } else if (IsGrounded() && GetVelocityX() != 0) {
        // if is walking
        // if facing right
        if (is_facing_right_)
            current_clip_ = &animations_[1][0];
        else
            current_clip_ = &animations_[1][1];
    } else if (!IsGrounded())
        current_clip_ = &animations_[2][0];  // falling
    else
        current_clip_ = &animations_[0][0];  // idle

    current_clip_->Draw(win, GetY(), draw_x);
}

void Actor::Update(int _vx, int _vy) {
    if (!is_dead_) {
        if (GetVelocityY() == 0 && _vy != 0 && IsGrounded()) {
            vy_ = _vy;
            is_grounded_ = false;
        } else if (!IsGrounded()) {
            ++vy_;
        }

        if (_vx != 0 && is_grounded_) {  // slower on ground
            if (_vx > 0)
                --_vx;
            else if (_vx < 0)
                ++_vx;
        }
        vx_ = _vx;

        // collision detection
        bool cOccured = false;
        if (GetVelocityY() > 0) {
            int VY = GetVelocityY();
            for (int v = 1; v <= VY && !cOccured; ++v) {  // start with vector with length 1
                vy_ = v;
                if (GetY() + height_ - 1 + v < kWindowHeight) {  // range checking
                    for (int c = GetX() + 1; c < GetX() + width_ - 1 && !cOccured;
                         ++c) {  // check if we can move by this vector
                        if (map[GetY() + height_ + v - 1][c] ==
                            'M') {  // if not, answer is vector 1
                                    // shorter otherwise we continue
                            vy_ = v - 1;
                            is_grounded_ = true;
                            cOccured = true;
                        }
                    }
                } else {
                    vy_ = v - 1;
                    cOccured = true;
                }
            }
        } else if (vy_ < 0) {
            int VY = GetVelocityY();
            for (int v = -1; v >= VY && !cOccured; --v) {
                vy_ = v;
                if (GetY() + v >= 0) {
                    for (int c = GetX() + 1; c < GetX() + width_ - 1 && !cOccured; ++c) {
                        if (map[GetY() + v][c] == 'M') {
                            vy_ = v + 1;
                            cOccured = true;
                        }
                    }
                } else {
                    vy_ = v + 1;
                    cOccured = true;
                }
            }
        } else {
            if (map[GetY() + height_][GetX() + 1] == ' ' &&
                map[GetY() + height_][GetX() + 2] == ' ')
                is_grounded_ = false;
        }
        // for x Axis it will be little more complicated
        // because we wile be taking into account shapes
        cOccured = false;
        if (GetVelocityX() > 0) {
            int VX = GetVelocityX();
            for (int v = 1; v <= VX && !cOccured; ++v) {
                vx_ = v;
                if (GetX() + width_ + v - 1 < kWindowWidth) {
                    for (int c = GetX() + width_ - 1; c >= GetX() && !cOccured; --c) {
                        for (int r = GetY(); r < GetY() + height_; ++r) {
                            if (map[r][c + v] == 'M' &&
                                animations_[0][0].GetFrames()[0][r - GetY()][c - GetX()] != ' ') {
                                vx_ = v - 1;
                                cOccured = true;
                            }
                        }
                    }
                } else {
                    vx_ = v - 1;
                    cOccured = true;
                }
            }
        } else if (GetVelocityX() < 0) {
            int VX = GetVelocityX();
            for (int v = -1; v >= VX && !cOccured; --v) {
                vx_ = v;
                if (GetX() + v >= 0) {
                    for (int c = GetX(); c < GetX() + width_ && !cOccured; ++c) {
                        for (int r = GetY(); r < GetY() + height_; ++r) {
                            if (map[r][c + v] == 'M' &&
                                animations_[0][0].GetFrames()[0][r - GetY()][c - GetX()] != ' ') {
                                vx_ = v + 1;
                                cOccured = true;
                            }
                        }
                    }
                } else {
                    vx_ = v + 1;
                    cOccured = true;
                }
            }
        }
        vy_ = CLAMP(GetVelocityY(), -3, 3);

        x_ += GetVelocityX();
        y_ += GetVelocityY();
        x_ = CLAMP(GetX(), 0, (int32_t)(kWindowWidth - width_));
        y_ = CLAMP(GetY(), 0, (int32_t)(kWindowHeight - height_ - 1));
    }
}

}  // namespace ascii_combat
