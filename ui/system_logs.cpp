#include <ncurses.h>
#include <string>
#include <vector>

#include "system.hpp"

using namespace std;

const int LINES_PER_PAGE = 50;

void displaySystemLogs(WINDOW* win, int ch) {
    static int current_line = 0;
    vector<string> logs = getSystemLogs();
    int num_lines = logs.size();
    
    if (ch == KEY_UP && current_line > 0) {
        current_line--;
    }
    else if (ch == KEY_DOWN && current_line + LINES_PER_PAGE < num_lines) {
        current_line++;
    }

    for (int i = 0; i < LINES_PER_PAGE && (current_line + i) < num_lines; ++i) {
        mvwprintw(win, i, 1, "%s", logs[current_line + i].c_str());
    }

}
