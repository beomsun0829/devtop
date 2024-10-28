#include <ncurses.h>
#include <unistd.h>

#include "ui/overview.hpp"
#include "ui/menubar.hpp"

#define FRAMES_PER_SECOND 30

int main() {
    initscr();
    if (has_colors()) {
        start_color();
    }

    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    int selected_menu = 0;
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    WINDOW* overview_win = newwin(max_y - 2, max_x, 0, 0);
    WINDOW* menu_win = newwin(2, max_x, max_y - 2, 0);
    
    int ch;
    while (true) {
        ch = getch();
        if (ch == 'q') {
            break;
        }
        else if (ch >= '1' && ch <= '6') {
            selected_menu = ch - '1';
        }

        werase(overview_win);
        werase(menu_win);

        if (selected_menu == 0) {
            runOverview(overview_win);
        }

        drawMenuBar(menu_win, selected_menu);

        wnoutrefresh(overview_win);
        wnoutrefresh(menu_win);
        doupdate();

        napms(1000 / FRAMES_PER_SECOND);
    }

    delwin(overview_win);
    delwin(menu_win);
    endwin();

    return 0;
}
