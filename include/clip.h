/* Copyright 2024 Damian Kolaska */
#pragma once

#include <ncurses.h>

#include <string>
#include <vector>

namespace ascii_combat {

using Frame = std::vector<std::string>;

class Clip {
   public:
    Clip(const std::vector<Frame>& frames, int32_t ticks_per_frame);
    void Draw(WINDOW* win, int32_t y, int32_t x);
    void SetTimer(int32_t timer);
    bool IsFinished() const;
    const std::vector<Frame>& GetFrames() const;

   private:
    std::vector<Frame> frames_;
    uint32_t current_frame_;
    int32_t timer_;
    int32_t ticks_per_frame_;
};

}  // namespace ascii_combat
