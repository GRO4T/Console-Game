/* Copyright 2024 Damian Kolaska */
#include "game.h"

#include <thread>  // NOLINT
#include <unordered_map>

#include "SFML/Window/Keyboard.hpp"
#include "config.h"

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

void Game::Update(const Input& input) {
    players_[0].Update(input, players_[1]);
    players_[1].Update(input, players_[0]);
}

void Game::Draw() {
    WINDOW* win = window_.GetHandle();
    wclear(win);
    // Draw map
    box(win, 0, 0);
    for (std::size_t i = 0; i < map_.size(); ++i) {
        mvwprintw(win, i, 1, "%s", map_[i].c_str());
    }
    // Draw players
    if (players_[1].IsDead()) {
        players_[1].Draw(win);
        players_[0].Draw(win);
    } else {
        players_[0].Draw(win);
        players_[1].Draw(win);
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

    DisplayTextCenter(window_, msg1);
    DisplayTextCenter(window_, msg2, 1);

    wgetch(window_.GetHandle());
}

Game::Answer Game::AskYesOrNo(const std::string& question) {
    DisplayTextCenter(window_, question);
    while (true) {
        char c = wgetch(window_.GetHandle());
        if (c == 'y' || c == 'Y') {
            return Answer::kYes;
        } else if (c == 'n' || c == 'N') {
            return Answer::kNo;
        }
    }
}

void Game::DisplayTextCenter(Window& window, const std::string& text, int32_t offset_y) {
    mvwprintw(window.GetHandle(), window.GetHeight() / 2 + offset_y,
              (window.GetWidth() - text.length()) / 2, text.c_str());
}

void Game::DisplayUI() {
    const auto player1_health = players_[0].GetHealth();
    const auto player2_health = players_[1].GetHealth();

    std::string health_bar1 =
        "health = " + std::to_string(player1_health >= 0 ? player1_health : 0);
    std::string health_bar2 =
        "health = " + std::to_string(player2_health >= 0 ? player2_health : 0);

    wattron(window_.GetHandle(), A_REVERSE);
    mvwprintw(window_.GetHandle(), 1, 1, "Player1");
    mvwprintw(window_.GetHandle(), 1, window_.GetWidth() - 7, "Player2");
    wattroff(window_.GetHandle(), A_REVERSE);
    mvwprintw(window_.GetHandle(), 2, 1, health_bar1.c_str());
    mvwprintw(window_.GetHandle(), 2, window_.GetWidth() - health_bar2.length(),
              health_bar2.c_str());
}

}  // namespace ascii_combat
