/* Copyright 2024 Damian Kolaska */
#include "window.h"

namespace ascii_combat {

Window::Window(uint32_t height, uint32_t width, int32_t top_left_x, int32_t top_left_y,
               uint32_t padding)
    : handle_(nullptr), height_(height), width_(width), padding_(padding) {
    initscr();
    noecho();     // do not echo characters back to terminal
    cbreak();     // stop line buffering, pass everything
    curs_set(0);  // make cursor invisible

    handle_ = newwin(height + 2 * padding, width + 2 * padding, top_left_x, top_left_y);

    nodelay(handle_, true);  // non-blocking input
    keypad(handle_, true);   // enable special characters
}

void Window::Clear() { wclear(handle_); }

void Window::Refresh() { wrefresh(handle_); }

void Window::DrawWindowBorder() { box(handle_, 0, 0); }

void Window::PrintRow(uint32_t y, const std::string& row) {
    mvwprintw(handle_, y + padding_, padding_, "%s", row.c_str());
}

void Window::PrintTextCenter(const std::string& text, int32_t offset_y) {
    auto draw_y = height_ / 2 + offset_y + padding_;
    auto draw_x = (width_ - text.length()) / 2 + padding_;
    mvwprintw(handle_, draw_y, draw_x, text.c_str());
}

void Window::PrintText(const std::string& text, uint32_t y, uint32_t x) {
    mvwprintw(handle_, y + padding_, x + padding_, text.c_str());
}

void Window::PrintCell(char cell, uint32_t y, uint32_t x) {
    mvwprintw(handle_, y + padding_, x + padding_, "%c", cell);
}

WINDOW* Window::GetHandle() { return handle_; }

Key Window::GetKey() {
    int key_code = wgetch(handle_);
    switch (key_code) {
        case KEY_UP:
            return Key::kUp;
        case KEY_DOWN:
            return Key::kDown;
        case 'y':
            return Key::kY;
        case 'n':
            return Key::kN;
        case 10:
            return Key::kEnter;
        default:
            return Key::kUnknown;
    }
}

uint32_t Window::GetHeight() const { return height_; }

uint32_t Window::GetWidth() const { return width_; }

uint32_t Window::GetPadding() const { return padding_; }

}  // namespace ascii_combat
