/* Copyright 2024 Damian Kolaska */
#include "game.h"

#include <thread>  // NOLINT
#include <unordered_map>

#include "SFML/Window/Keyboard.hpp"
#include "config.h"
#include "event_system.h"

using namespace std::chrono_literals;  // NOLINT

namespace ascii_combat {

Game::Game(Window& window, const std::vector<std::string>& map,
           const std::vector<Player::Controls>& player_controls_list)
    : window_(window),
      state_(State::kRunning),
      map_(map),
      players_({PlayerFactory::CreatePlayer(0, 0, true, player_controls_list[0], map),
                PlayerFactory::CreatePlayer(76, 0, false, player_controls_list[1], map)}) {}

void Game::GameLoop() {
    while (true) {
        const auto start = std::chrono::system_clock::now();

        if (State::kPaused == state_) {
            std::string question = "Do you really want to quit? (Y/N)";
            auto answer = AskYesOrNo(question);
            if (answer == Answer::kYes) {
                return;
            } else {
                state_ = State::kRunning;
            }
        }

        const auto input = GetInput();
        ProcessEvents();
        Update(input);
        Draw();

        const auto end = std::chrono::system_clock::now();
        const auto elapsed_time = end - start;

        std::this_thread::sleep_for(4 * 16.7ms - elapsed_time);

        if (players_[0].IsDead() || players_[1].IsDead()) {
            End();
            return;
        }

        wrefresh(window_.GetHandle());
    }
}

Input Game::GetInput() {
    Input input;

    auto SetKeyState = [&](const auto key, const auto key_sfml) {
        auto key_state =
            sf::Keyboard::isKeyPressed(key_sfml) ? KeyState::kPressed : KeyState::kReleased;
        input.insert({key, key_state});
    };

    SetKeyState(Key::kW, sf::Keyboard::W);
    SetKeyState(Key::kS, sf::Keyboard::S);
    SetKeyState(Key::kA, sf::Keyboard::A);
    SetKeyState(Key::kD, sf::Keyboard::D);
    SetKeyState(Key::kUp, sf::Keyboard::Up);
    SetKeyState(Key::kDown, sf::Keyboard::Down);
    SetKeyState(Key::kLeft, sf::Keyboard::Left);
    SetKeyState(Key::kRight, sf::Keyboard::Right);
    SetKeyState(Key::kQ, sf::Keyboard::Q);

    return input;
}

void Game::ProcessEvents() {
    auto event = EventSystem::Instance().ConsumeEvent();
    while (event != nullptr) {
        auto player_attack_event = dynamic_cast<PlayerAttackEvent*>(event.get());
        if (player_attack_event != nullptr) {
            Handle(*player_attack_event);
        }
        event = EventSystem::Instance().ConsumeEvent();
    }
}

void Game::Update(const Input& input) {
    if (KeyState::kPressed == input.at(Key::kQ)) {
        state_ = State::kPaused;
    }

    players_[0].Update(input, players_[1]);
    players_[1].Update(input, players_[0]);
}

void Game::Draw() {
    window_.Clear();
    window_.DrawWindowBorder();
    // Draw map
    for (std::size_t y = 0; y < map_.size(); ++y) {
        window_.PrintRow(y, map_[y]);
    }
    // Draw players
    if (players_[1].IsDead()) {
        players_[1].Draw(window_);
        players_[0].Draw(window_);
    } else {
        players_[0].Draw(window_);
        players_[1].Draw(window_);
    }
    DisplayUI();
}

void Game::End() {
    std::string msg1;
    const std::string msg2 = "Press any key to continue...";

    if (players_[0].IsDead() && players_[1].IsDead())
        msg1 = "It's a draw!";
    else if (players_[0].IsDead())
        msg1 = "Player2 won!";
    else if (players_[1].IsDead())
        msg1 = "Player1 won!";

    window_.PrintTextCenter(msg1);
    window_.PrintTextCenter(msg2, 1);

    wgetch(window_.GetHandle());
}

void Game::Handle(const PlayerAttackEvent& event) {
    const auto& attacker = event.attacker;
    auto& target = event.target;

    bool is_facing_right_direction =
        (attacker.IsFacingRight() && attacker.GetX() < target.GetX()) ||
        (!attacker.IsFacingRight() && attacker.GetX() > target.GetX());
    bool is_target_in_range = abs(target.GetY() - attacker.GetY()) <= 1 &&
                              abs(target.GetX() - attacker.GetX()) <=
                                  (int32_t)(attacker.GetAttackRange() + attacker.GetWidth());

    if (is_facing_right_direction && is_target_in_range) {
        auto attacked_from =
            attacker.GetX() < target.GetX() ? Player::Direction::kLeft : Player::Direction::kRight;
        target.GotHit(event.damage, attacked_from);
    }
}

Game::Answer Game::AskYesOrNo(const std::string& question) {
    window_.PrintTextCenter(question);
    while (true) {
        Key key = window_.GetKey();
        if (Key::kY == key) {
            return Answer::kYes;
        } else if (Key::kN == key) {
            return Answer::kNo;
        }
    }
}

void Game::DisplayUI() {
    std::string player1_name = "Player1";
    std::string player2_name = "Player2";
    auto player1_health = players_[0].GetHealth();
    auto player2_health = players_[1].GetHealth();

    std::string player1_health_text =
        "health = " + std::to_string(player1_health >= 0 ? player1_health : 0);
    std::string player2_health_text =
        "health = " + std::to_string(player2_health >= 0 ? player2_health : 0);

    window_.PrintText(player1_name, 0, 0);
    window_.PrintText(player1_health_text, 1, 0);

    auto player2_stats_draw_x =
        window_.GetWidth() - std::max(player2_name.length(), player2_health_text.length()) - 1;
    window_.PrintText(player2_name, 0, player2_stats_draw_x);
    window_.PrintText(player2_health_text, 1, player2_stats_draw_x);
}

}  // namespace ascii_combat
