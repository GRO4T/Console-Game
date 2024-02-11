#include "player.h"

#include "config.h"

namespace ascii_combat {

Player::Player(int x, int y, int width, int height, int speed, int jump_force, int attack_range,
               int health, bool is_facing_right, const KeyMapping& key_map, const Map& map,
               std::vector<std::vector<Clip>>& anims)
    : x_(x),
      y_(y),
      vx_(0),
      vy_(0),
      width_(width),
      height_(height),
      speed_(speed),
      jump_force_(jump_force),
      health_(health),
      is_facing_right_(is_facing_right),
      is_attacking_(false),
      is_dead_(false),
      attack_range_(attack_range),
      key_map_(key_map),
      map_(map) {
    for (auto anim : anims) {
        animations_.emplace_back(anim);
    }
    current_clip_ = &animations_[0][0];
    Move(0, 0);  // TODO: Is it necessary?
}

int32_t Player::GetX() const { return x_; }

int32_t Player::GetY() const { return y_; }

int32_t Player::GetSpeed() const { return speed_; }

int32_t Player::GetJumpForce() const { return jump_force_; }

uint32_t Player::GetAttackRange() const { return attack_range_; }

uint32_t Player::GetWidth() const { return width_; }

int32_t Player::GetHealth() const { return health_; }

const std::vector<std::vector<Clip>>& Player::GetAnimations() const { return animations_; }

Clip* Player::GetCurrentClip() const { return current_clip_; }

bool Player::IsGrounded() const { return is_grounded_; }

bool Player::IsFacingRight() const { return is_facing_right_; }

bool Player::IsAttacking() const { return is_attacking_; }

bool Player::IsDead() const { return is_dead_; }

void Player::SetIsFacingRight(bool value) { is_facing_right_ = value; }

void Player::SetIsAttacking(bool value) { is_attacking_ = value; }

void Player::Draw(WINDOW* win) {
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
    } else if (IsGrounded() && vx_ != 0) {
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

void Player::Update(const Input& input, Player& opponent) {
    int dx = 0;
    int dy = 0;

    bool is_down_pressed = input.at(key_map_.down) == KeyState::kPressed;
    bool is_up_pressed = input.at(key_map_.up) == KeyState::kPressed;
    bool is_left_pressed = input.at(key_map_.left) == KeyState::kPressed;
    bool is_right_pressed = input.at(key_map_.right) == KeyState::kPressed;

    if (is_down_pressed && is_grounded_) {
        is_attacking_ = true;
    } else if (is_up_pressed) {
        dy -= jump_force_;
    } else if (is_left_pressed && !is_attacking_) {
        dx -= speed_;
        is_facing_right_ = false;
    } else if (is_right_pressed && !is_attacking_) {
        dx += speed_;
        is_facing_right_ = true;
    }

    if (is_attacking_) {
        if (current_clip_->IsFinished()) {
            Attack(opponent);
        }
        return;
    }

    Move(dx, dy);
}

void Player::Move(int32_t dx, int32_t dy) {
    if (!is_dead_) {
        if (vy_ == 0 && dy != 0 && IsGrounded()) {
            vy_ = dy;
            is_grounded_ = false;
        } else if (!IsGrounded()) {
            ++vy_;
        }

        if (dx != 0 && is_grounded_) {  // slower on ground
            if (dx > 0)
                --dx;
            else if (dx < 0)
                ++dx;
        }
        vx_ = dx;

        // collision detection
        bool cOccured = false;
        if (vy_ > 0) {
            int VY = vy_;
            for (int v = 1; v <= VY && !cOccured; ++v) {  // start with vector with length 1
                vy_ = v;
                if (GetY() + height_ - 1 + v < kWindowHeight) {  // range checking
                    for (int c = GetX() + 1; c < GetX() + width_ - 1 && !cOccured;
                         ++c) {  // check if we can move by this vector
                        if (map_[GetY() + height_ + v - 1][c] ==
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
            int VY = vy_;
            for (int v = -1; v >= VY && !cOccured; --v) {
                vy_ = v;
                if (GetY() + v >= 0) {
                    for (int c = GetX() + 1; c < GetX() + width_ - 1 && !cOccured; ++c) {
                        if (map_[GetY() + v][c] == 'M') {
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
            if (map_[GetY() + height_][GetX() + 1] == ' ' &&
                map_[GetY() + height_][GetX() + 2] == ' ')
                is_grounded_ = false;
        }
        // for x Axis it will be little more complicated
        // because we wile be taking into account shapes
        cOccured = false;
        if (vx_ > 0) {
            int VX = vx_;
            for (int v = 1; v <= VX && !cOccured; ++v) {
                vx_ = v;
                if (GetX() + width_ + v - 1 < kWindowWidth) {
                    for (int c = GetX() + width_ - 1; c >= GetX() && !cOccured; --c) {
                        for (int r = GetY(); r < GetY() + height_; ++r) {
                            if (map_[r][c + v] == 'M' &&
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
        } else if (vx_ < 0) {
            int VX = vx_;
            for (int v = -1; v >= VX && !cOccured; --v) {
                vx_ = v;
                if (GetX() + v >= 0) {
                    for (int c = GetX(); c < GetX() + width_ && !cOccured; ++c) {
                        for (int r = GetY(); r < GetY() + height_; ++r) {
                            if (map_[r][c + v] == 'M' &&
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
        vy_ = CLAMP(vy_, -3, 3);

        x_ += vx_;
        y_ += vy_;
        x_ = CLAMP(GetX(), 0, (int32_t)(kWindowWidth - width_));
        y_ = CLAMP(GetY(), 0, (int32_t)(kWindowHeight - height_ - 1));
    }
}

void Player::Attack(Player& opponent) {
    if (IsInRange(this, &opponent)) {
        opponent.GotHit();
        if (opponent.IsDead()) {
        } else {
            if (GetX() > opponent.GetX())
                opponent.Move(-1, -1);
            else
                opponent.Move(1, -1);
        }
    }
}

bool Player::IsInRange(Player* player, Player* target) {
    if ((player->IsFacingRight() && player->GetX() < target->GetX()) ||
        (!player->IsFacingRight() &&
         player->GetX() > target->GetX())) {  // if pointing towards enemy
        if (abs(target->GetY() - player->GetY()) <= 1 &&
            abs(target->GetX() - player->GetX()) <= player->GetAttackRange() + player->GetWidth())
            return true;
    }
    return false;
}

Player PlayerFactory::CreatePlayer(int x, int y, bool is_facing_right, const KeyMapping& key_map,
                                   const Map& map) {
    return Player(x, y, kPlayerWidth, kPlayerHeight, 3, 3, kPlayerAttackRange, kPlayerHealth,
                  is_facing_right, key_map, map, Assets::Instance().GetPlayerAnimations());
}

}  // namespace ascii_combat
