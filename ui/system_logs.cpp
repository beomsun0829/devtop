#include <ncurses.h>
#include <string>
#include <vector>
#include <regex>
#include <ctime>
#include "system.hpp"

using namespace std;

const int INITIAL_LOAD_SIZE = 100;
const int MAX_LOG_LEVEL = 4;

void displaySystemLogs(WINDOW* win, int ch) {
    static int current_line = 0;
    static vector<string> logs;
    static int num_lines = 0;
    static bool initialized = false;
    static time_t last_fetch_time = 0;
    static int log_level = -1;
    static int load_size = INITIAL_LOAD_SIZE;

    time_t current_time = time(nullptr);

    if (logs.empty() || difftime(current_time, last_fetch_time) >= 5) {
        logs = getSystemLogs(load_size, log_level);
        num_lines = logs.size();
        last_fetch_time = current_time;

        if (!initialized && num_lines > load_size) {
            current_line = num_lines - load_size;
            initialized = true;
        }
    }

    if (ch == KEY_F(1)) log_level = 0;
    else if (ch == KEY_F(2)) log_level = 1;
    else if (ch == KEY_F(3)) log_level = 2;
    else if (ch == KEY_F(4)) log_level = 3;
    else if (ch == KEY_F(5)) log_level = 4;
    else if (ch == KEY_F(6)) log_level = -1;

    if (ch >= KEY_F(1) && ch <= KEY_F(6)) {
        load_size = INITIAL_LOAD_SIZE;
        logs = getSystemLogs(load_size, log_level);
        num_lines = logs.size();
        current_line = 0;
    }

    if (ch == KEY_UP && current_line > 0) {
        current_line--;
    } else if (ch == KEY_UP && current_line == 0) {
        int previous_num_lines = num_lines;
        load_size += INITIAL_LOAD_SIZE; 
        logs = getSystemLogs(load_size, log_level);
        num_lines = logs.size();

        current_line += (num_lines - previous_num_lines);
    } else if (ch == KEY_DOWN && current_line + 1 < num_lines) {
        current_line++;
    }

    werase(win);

    int max_y, max_x;
    getmaxyx(win, max_y, max_x);

    mvwprintw(win, 0, 1, "F1-F5: Set Log Level | F6: Clear Filter | UP/DOWN: Scroll");

    regex timestamp_regex(R"(\b\w{3} \d{2} \d{2}:\d{2}:\d{2})");
    regex warning_regex(R"(\b(warning|warn|WARN|WARNING)\b)");
    regex error_regex(R"(\b(error|fail|failed|ERROR|FAIL)\b)");

    for (int i = 0; i < max_y - 1 && (current_line + i) < num_lines; i++) {
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

        //string message_type = "[INFO]";
        //if (regex_search(log, error_regex)) message_type = "[ERROR]";
        //else if (regex_search(log, warning_regex)) message_type = "[WARN]";

        //string formatted_log = timestamp + " " + message_type + " " + log;
        string formatted_log = timestamp + " " + log;

        if (formatted_log.length() > max_x - 1) {
            formatted_log = formatted_log.substr(0, max_x - 1);
        }

        mvwprintw(win, i + 1, 0, "%s", formatted_log.c_str());
    }

    wrefresh(win);
}

