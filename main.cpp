#include <ncurses.h>
#include <unistd.h>

#include "ui/overview.hpp"
#include "ui/menubar.hpp"
#include "ui/system_logs.hpp"

#define FRAMES_PER_SECOND 30

void run_menu(int selected_menu, WINDOW* main_win, int ch);

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

    WINDOW* main_win = newwin(max_y - 2, max_x, 0, 0);
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
        
        werase(main_win);
        werase(menu_win);
        
        run_menu(selected_menu, main_win, ch);

        drawMenuBar(menu_win, selected_menu);

        wnoutrefresh(main_win);
        wnoutrefresh(menu_win);
        doupdate();

        napms(1000 / FRAMES_PER_SECOND);
    }

    delwin(main_win);
    delwin(menu_win);
    endwin();

    return 0;
}

void run_menu(int selected_menu, WINDOW* main_win, int ch){
    switch(selected_menu){
        case 0:
            displayOverview(main_win);
            break;

        case 5:
            displaySystemLogs(main_win, ch);
            break;

        default:
            displayOverview(main_win);
            break;
    }
}
