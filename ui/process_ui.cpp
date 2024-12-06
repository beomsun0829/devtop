#include <ncurses.h>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include "process.hpp"

void displayProcessUI(WINDOW* win) {
    int max_y, max_x;
    getmaxyx(win, max_y, max_x);
    int header_height = 3;
    int table_height = max_y - header_height - 2;

    werase(win);
    ProcessCount proc_count = getProcessCount();
    LoadAvg load_avg = getLoadAvg();
    std::vector<ProcessDetails> processes = getTopProcessesDetails();

    mvwprintw(win, 1, 1, "Processes: Total: %d | Running: %d | Sleeping: %d | Load Avg: %.2f %.2f %.2f",
            proc_count.total, proc_count.running, proc_count.sleeping,
            load_avg.one_min, load_avg.five_min, load_avg.fifteen_min);

    mvwprintw(win, 3, 1, "PID    PGID    COMMAND         CPU%%   MEM%%   STATE   THREADS  USER       START TIME");

    for (int i = 0; i < table_height && i < processes.size(); i++) {
        ProcessDetails& proc = processes[i];
        mvwprintw(win, 4 + i, 1, "%-6d %-6d %-15s %-6.2f %-6.2f %-6s %-8d %-10s %s",
                  proc.pid, proc.pgid, proc.command.c_str(), proc.cpu_usage,
                  proc.memory_usage, proc.state.c_str(), proc.threads, proc.user.c_str(),
                  proc.start_time.c_str());
    }
    wrefresh(win);
    usleep(100000);
    
}

