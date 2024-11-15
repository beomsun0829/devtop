#include <ncurses.h>
#include <string>
#include <vector>

#include "menubar.hpp"

using namespace std;

void drawMenuBar(WINDOW* win, int selected) {
    int win_max_y, win_max_x;
    getmaxyx(win, win_max_y, win_max_x);

    vector<string> menu_items = {
        "1. Overview", "2. Process", "3. Network", "4. Hardware", "5. System Logs"
    };

    int num_items = menu_items.size();
    int item_width = win_max_x / num_items;

    for (int i = 0; i < num_items; i++) {
        if (i == selected) {
            wattron(win, A_REVERSE);
        }

        mvwprintw(win, 0, i * item_width + 1, "%-*s", item_width - 1, menu_items[i].c_str());

        if (i == selected) {
            wattroff(win, A_REVERSE);
        }
    }
    wrefresh(win);
}
