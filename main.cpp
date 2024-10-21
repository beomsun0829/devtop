#include <ncurses.h>
#include "ui/overview.hpp"

int main() {
    initscr();
    if (has_colors()) {
        start_color();
    }

    noecho();
    cbreak();
    timeout(500);

    runOverview();

    endwin();
    return 0;
}
