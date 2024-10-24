#include <ncurses.h>
#include <string>
#include <vector>
#include "menubar.hpp"

using namespace std;

void drawMenuBar(WINDOW* win, int selected) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int menu_y = max_y - 2;

    vector<string> menu_items = {
        "Overview", "Process", "Network", "I/O Disk", "Hardware", "System Logs"
    };

    int num_items = menu_items.size();
    int item_width = max_x / num_items;

    for (int i = 0; i < num_items; i++) {
        if (i == selected) {
            wattron(win, A_REVERSE);
        }

        mvwprintw(win, menu_y, i * item_width + 1, "%-*s", item_width - 1, menu_items[i].c_str());

        if (i == selected) {
            wattroff(win, A_REVERSE);
        }
    }

    wrefresh(win);
}
