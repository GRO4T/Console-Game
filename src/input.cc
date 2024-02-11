#include "input.h"

namespace ascii_combat {

bool WaitForInput(timeval timeout) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);  // REMINDER: In UNIX, file descriptor 0 corresponds to standard input
    return select(1, &readfds, nullptr, nullptr, &timeout);
}

}  // namespace ascii_combat