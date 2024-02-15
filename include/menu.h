/* Copyright 2024 Damian Kolaska */
#pragma once

#include <ncurses.h>

#include <string>
#include <vector>

namespace ascii_combat {

using Choice = std::string;

class Menu {
   public:
    Menu(WINDOW* window, const std::vector<Choice>& choices);

    const Choice& GetChoice();

   private:
    WINDOW* window_;
    std::vector<Choice> choices_;
    int highlighted_;

    void Display();
};

}  // namespace ascii_combat
