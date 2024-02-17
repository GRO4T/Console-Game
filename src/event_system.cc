/* Copyright 2024 Damian Kolaska */
#include "event_system.h"

namespace ascii_combat {

// NOTE: This is not thread safe.
EventSystem& EventSystem::Instance() {
    static EventSystem instance;
    return instance;
}

void EventSystem::CreateEvent(std::unique_ptr<Event> event) { events_.push(std::move(event)); }

std::unique_ptr<Event> EventSystem::ConsumeEvent() {
    if (events_.empty()) {
        return nullptr;
    }
    auto event = std::move(events_.front());
    events_.pop();
    return event;
}

}  // namespace ascii_combat
