#include "clip.h"

namespace ascii_combat {

Clip::Clip(const std::vector<Frame>& frames, int32_t ticks_per_frame)
    : frames_(frames),
      current_frame_(0),
      timer_(ticks_per_frame),
      ticks_per_frame_(ticks_per_frame) {}

void Clip::Draw(WINDOW* win, int32_t y, int32_t x) {
    const auto& f = frames_[current_frame_];

    for (std::size_t row = 0; row < f.size(); ++row) {
        for (std::size_t col = 0; col < f[row].size(); ++col) {
            const char cell = f[row][col];
            if (cell != ' ') {
                mvwprintw(win, y + row, x + col, "%c", cell);
            }
        }
    }

    --timer_;
    if (timer_ <= 0) {
        timer_ = ticks_per_frame_;
        current_frame_ = (current_frame_ + 1) % frames_.size();
    }
}

void Clip::SetTimer(int32_t timer) { timer_ = timer; }

bool Clip::IsFinished() const { return current_frame_ == frames_.size() - 1; }

const std::vector<Frame>& Clip::GetFrames() const { return frames_; }

}  // namespace ascii_combat