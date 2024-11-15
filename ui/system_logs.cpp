#include <ncurses.h>
#include <string>
#include <vector>
#include <regex>
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
    } else if (ch == KEY_DOWN && current_line + LINES_PER_PAGE < num_lines) {
        current_line++;
    }

    werase(win);

    int max_y, max_x;
    getmaxyx(win, max_y, max_x);

    regex timestamp_regex(R"(\b\w{3} \d{2} \d{2}:\d{2}:\d{2})");
    regex warning_regex(R"(\b(warning|warn|WARN|WARNING)\b)");
    regex error_regex(R"(\b(error|fail|failed|ERROR|FAIL)\b)");

    for (int i = 0; i < LINES_PER_PAGE && (current_line + i) < num_lines; i++) {
        string log = logs[current_line + i];

        string timestamp = " ";
        smatch match;
        if (regex_search(log, match, timestamp_regex)) {
            timestamp = match.str();
            log = log.substr(match.position() + match.length());
        }

        size_t user_pos = log.find("user-");
        string username = (user_pos != string::npos) ? "user" : "";
        log = log.substr(log.find_first_of(" ", user_pos + 5) + 1);

        string message_type = "[INFO]";
        if (regex_search(log, error_regex)) {
            message_type = "[ERROR]";
        } else if (regex_search(log, warning_regex)) {
            message_type = "[WARN]";
        }

        string formatted_log = timestamp + " " + message_type + " " + log;

        if (formatted_log.length() > max_x - 1) {
            formatted_log = formatted_log.substr(0, max_x - 1);
        }

        mvwprintw(win, i + 1, 0, "%s", formatted_log.c_str());
    }

    wrefresh(win);
}
