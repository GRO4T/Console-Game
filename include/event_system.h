/* Copyright 2024 Damian Kolaska */
#pragma once

#include <memory>
#include <queue>

#include "event.h"

namespace ascii_combat {

class EventSystem {
   public:
    EventSystem(const EventSystem& other) = delete;
    void operator=(const EventSystem&) = delete;

    static EventSystem& Instance();

    void CreateEvent(std::unique_ptr<Event> event);
    std::unique_ptr<Event> ConsumeEvent();

   private:
    EventSystem() = default;
    ~EventSystem() = default;

    std::queue<std::unique_ptr<Event>> events_;
};

}  // namespace ascii_combat
