#include "menu.h"

#include "config.h"

namespace ascii_combat {

Menu::Menu(WINDOW* window, const std::vector<Choice>& choices)
    : window_(window), choices_(choices), highlighted_(0) {}

const Choice& Menu::GetChoice() {
    wclear(window_);
    Display();

    while (true) {
        int key = wgetch(window_);

        switch (key) {
            case KEY_UP:
                if (0 == highlighted_)
                    highlighted_ = choices_.size();
                else
                    --highlighted_;
                break;
            case KEY_DOWN:
                if (choices_.size() == (std::size_t)highlighted_)
                    highlighted_ = 0;
                else
                    ++highlighted_;
                break;
            case 10:
                return choices_[highlighted_];
        }

        Display();
    }
}

void Menu::Display() {
    int y = (kWindowHeight - 2) / 2;

    box(window_, 0, 0);

    for (std::size_t i = 0; i < choices_.size(); ++i) {
        int x = (kWindowWidth - choices_[i].size()) / 2;
        if (i == (std::size_t)highlighted_) {
            wattron(window_, A_REVERSE);
            mvwprintw(window_, y, x, "%s", choices_[i].c_str());
            wattroff(window_, A_REVERSE);
        } else {
            mvwprintw(window_, y, x, "%s", choices_[i].c_str());
        }
        y += 2;
    }

    wrefresh(window_);
}

}  // namespace ascii_combat