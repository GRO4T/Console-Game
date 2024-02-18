/* Copyright 2024 Damian Kolaska */
#pragma once

#include <ncurses.h>
#include <stdint.h>

#include <string>

#include "input.h"

namespace ascii_combat {

class Window {
   public:
    Window(uint32_t height, uint32_t width, int32_t top_left_x, int32_t top_left_y,
           uint32_t padding);
    ~Window();

    WINDOW* GetHandle();
    void Clear();
    void Refresh();
    Key GetKey();
    void DrawWindowBorder();

    uint32_t GetHeight() const;
    uint32_t GetWidth() const;
    uint32_t GetPadding() const;

    void PrintRow(uint32_t y, const std::string& row);
    void PrintTextCenter(const std::string& text, int32_t offset_y = 0);
    void PrintText(const std::string& text, uint32_t y, uint32_t x);
    void PrintCell(char cell, uint32_t y, uint32_t x);

   private:
    WINDOW* handle_;
    uint32_t height_;
    uint32_t width_;
    uint32_t padding_;
};

}  // namespace ascii_combat
