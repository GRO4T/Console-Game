/* Copyright 2024 Damian Kolaska */
#include "window.h"

namespace ascii_combat {

Window::Window(uint32_t height, uint32_t width, int32_t top_left_x, int32_t top_left_y,
               uint32_t padding)
    : height_(height), width_(width) {
    initscr();
    noecho();     // do not echo characters back to terminal
    cbreak();     // stop line buffering, pass everything
    curs_set(0);  // make cursor invisible

    handle_ = newwin(height + padding, width + padding, top_left_x, top_left_y);

    nodelay(handle_, true);  // non-blocking input
    keypad(handle_, true);   // enable special characters
}

WINDOW* Window::GetHandle() { return handle_; }

uint32_t Window::GetHeight() const { return height_; }

uint32_t Window::GetWidth() const { return width_; }

}  // namespace ascii_combat
