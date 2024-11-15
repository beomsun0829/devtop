#include <ncurses.h>
#include <string>
#include <vector>
#include <regex>
#include <ctime>
#include "system.hpp"

using namespace std;

const int LOAD_SIZE = 100;
const int MAX_LOG_LEVEL = 4;

void displaySystemLogs(WINDOW* win, int ch) {
    static int current_line = 0;
    static vector<string> logs;
    static int num_lines = 0;
    static bool initialized = false;
    static time_t last_fetch_time = 0;
    static int log_level = -1;
    static string search_keyword = "";
    static bool search_mode = false;

    time_t current_time = time(nullptr);

    if (logs.empty() || difftime(current_time, last_fetch_time) >= 5) {
        logs = getSystemLogs(LOAD_SIZE, log_level, search_keyword);
        num_lines = logs.size();
        last_fetch_time = current_time;

        if (!initialized && num_lines > LOAD_SIZE) {
            current_line = num_lines - LOAD_SIZE;
            initialized = true;
        }
    }

    if (search_mode) {
        if (ch == '\n') {
            search_mode = false;
            logs = getSystemLogs(LOAD_SIZE, log_level, search_keyword);
            num_lines = logs.size();
            current_line = 0;
        } else if (ch == KEY_BACKSPACE || ch == 127) {
            if (!search_keyword.empty()) search_keyword.pop_back();
        } else if (isprint(ch)) {
            search_keyword += static_cast<char>(ch);
        }
    } else {
        if (ch == KEY_F(1)) log_level = 0;
        else if (ch == KEY_F(2)) log_level = 1;
        else if (ch == KEY_F(3)) log_level = 2;
        else if (ch == KEY_F(4)) log_level = 3;
        else if (ch == KEY_F(5)) log_level = 4;
        else if (ch == KEY_F(6)) log_level = -1;
        else if (ch == KEY_F(7)) {
            search_mode = true;
            search_keyword.clear();
            werase(win);
            mvwprintw(win, getmaxy(win) - 1, 1, "Search: ");
            wrefresh(win);
            return;
        }

        if (ch >= KEY_F(1) && ch <= KEY_F(6)) {
            logs = getSystemLogs(LOAD_SIZE, log_level, search_keyword);
            num_lines = logs.size();
            current_line = 0;
        }
    }

    if (ch == KEY_UP && current_line > 0) current_line--;
    else if (ch == KEY_DOWN && current_line + LOAD_SIZE < num_lines) current_line++;

    werase(win);

    int max_y, max_x;
    getmaxyx(win, max_y, max_x);

    mvwprintw(win, 0, 1, "F1-F5: Set Log Level | F6: Clear Filter | F7: Search | UP/DOWN: Scroll");

    regex timestamp_regex(R"(\b\w{3} \d{2} \d{2}:\d{2}:\d{2})");
    regex warning_regex(R"(\b(warning|warn|WARN|WARNING)\b)");
    regex error_regex(R"(\b(error|fail|failed|ERROR|FAIL)\b)");

    for (int i = 0; i < LOAD_SIZE && (current_line + i) < num_lines; i++) {
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
        if (regex_search(log, error_regex)) message_type = "[ERROR]";
        else if (regex_search(log, warning_regex)) message_type = "[WARN]";

        string formatted_log = timestamp + " " + message_type + " " + log;

        if (formatted_log.length() > max_x - 1) {
            formatted_log = formatted_log.substr(0, max_x - 1);
        }

        mvwprintw(win, i + 1, 0, "%s", formatted_log.c_str());
    }

    if (search_mode) mvwprintw(win, max_y - 1, 1, "Search: %s", search_keyword.c_str());

    wrefresh(win);
}
