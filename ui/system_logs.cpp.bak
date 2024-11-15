#include <ncurses.h>
#include <string>
#include <vector>
#include <ctime>

#include "system.hpp"

using namespace std;

const int LINES_PER_PAGE = 50;

void displaySystemLogs(WINDOW* win, int ch) {
    static int current_line = 0;
    static vector<string> logs;
    static int num_lines = 0;
    static time_t last_fetch_time = 0;

    time_t current_time = time(nullptr);

    if (logs.empty() || difftime(current_time, last_fetch_time) >= 5) {
        logs = getSystemLogs();
        num_lines = logs.size();
        last_fetch_time = current_time;
    }

    if (ch == KEY_UP && current_line > 0) {
        current_line--;
    }
    else if (ch == KEY_DOWN && current_line + LINES_PER_PAGE < num_lines) {
        current_line++;
    }

    werase(win);

    for (int i = 0; i < LINES_PER_PAGE && (current_line + i) < num_lines; i++) {
        mvwprintw(win, i, 1, "%s", logs[current_line + i].c_str());
    }

    wrefresh(win);
}
