#include <ncurses.h>
#include "ui/overview.hpp"
#include "ui/menubar.hpp"

int main() {
    initscr();
    if (has_colors()) {
        start_color();
    }

    noecho();
    cbreak();
    timeout(500);

    int selected_menu = 0;
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    WINDOW* win = newwin(max_y, max_x, 0, 0);

    while (true) {
        werase(win);
        runOverview();
        drawMenuBar(win, selected_menu);

        int ch = getch();
        if (ch == 'q') {
            break;
        }
        else if (ch == KEY_LEFT) {
            selected_menu = (selected_menu == 0) ? selected_menu : selected_menu - 1;
        }
        else if (ch == KEY_RIGHT) {
            selected_menu = (selected_menu == 5) ? selected_menu : selected_menu + 1;
        }
        wrefresh(win);
    }
    delwin(win);
    endwin();

    return 0;
}
