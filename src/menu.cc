/* Copyright 2024 Damian Kolaska */
#include "menu.h"

#include "config.h"

namespace ascii_combat {

Menu::Menu(Window& window, const std::vector<Choice>& choices)
    : window_(window), choices_(choices), highlighted_(0) {}

const Choice& Menu::GetChoice() {
    window_.Clear();
    while (true) {
        Display();
        Key key = window_.GetKey();

        if (Key::kUp == key) {
            if (0 == highlighted_)
                highlighted_ = choices_.size();
            else
                --highlighted_;
        } else if (Key::kDown == key) {
            if (choices_.size() == (std::size_t)highlighted_)
                highlighted_ = 0;
            else
                ++highlighted_;
        } else if (Key::kEnter == key) {
            return choices_[highlighted_];
        }
    }
}

void Menu::Display() {
    window_.DrawWindowBorder();

    for (std::size_t i = 0; i < choices_.size(); ++i) {
        auto offset_y = 2 * (i - choices_.size() / 2);
        if (i == (std::size_t)highlighted_) {
            wattron(window_.GetHandle(), A_REVERSE);
            window_.PrintTextCenter(choices_[i], offset_y);
            wattroff(window_.GetHandle(), A_REVERSE);
        } else {
            window_.PrintTextCenter(choices_[i], offset_y);
        }
    }

    window_.Refresh();
}

}  // namespace ascii_combat
