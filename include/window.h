/* Copyright 2024 Damian Kolaska */
#pragma once

#include <ncurses.h>

namespace ascii_combat {

class Window {
   public:
    Window(uint32_t height, uint32_t width, int32_t top_left_x, int32_t top_left_y,
           uint32_t padding);
    WINDOW* GetHandle();
    uint32_t GetHeight() const;
    uint32_t GetWidth() const;

   private:
    WINDOW* handle_;
    uint32_t height_;
    uint32_t width_;
};

}  // namespace ascii_combat
