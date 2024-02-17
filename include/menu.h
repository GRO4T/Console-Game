/* Copyright 2024 Damian Kolaska */
#pragma once

#include <string>
#include <vector>

#include "window.h"

namespace ascii_combat {

using Choice = std::string;

class Menu {
   public:
    Menu(Window& window, const std::vector<Choice>& choices);  // NOLINT

    const Choice& GetChoice();

   private:
    Window& window_;
    std::vector<Choice> choices_;
    int highlighted_;

    void Display();
};

}  // namespace ascii_combat
