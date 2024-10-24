#include <iostream>
#include <ncurses.h>
#include <cstdio>
#include <unistd.h>
#include <vector>

#include "cpu.hpp"
#include "memory.hpp"
#include "format.hpp"
#include "user.hpp"
#include "system.hpp"
#include "disk.hpp"
#include "network.hpp"
#include "process.hpp"

void displayOverview(WINDOW* win) {
    werase(win);
    int terminal_width = getmaxx(win);    
    int text_width = 12;

    CPU cpu;
    cpu.getUsage();
    getDiskUsage("/");
    usleep(100000);

    float cpu_usage = cpu.getUsage();
    float mem_usage = getMemoryUsage();
    float disk_usage = getDiskUsage("/");
    string active_interface;
    NetworkStats net_stats = getMainNetworkUsage(active_interface);
    ProcessCount proc_info = getProcessCount();
    LoadAvg load_avg = getLoadAvg();
    vector<ProcessInfo> top_processes = getTopProcesses();

    mvwprintw(win, 1, 1, "%-*s %s", text_width, "User", getCurrentUser().c_str());
    mvwprintw(win, 2, 1, "%-*s %s", text_width, "Uptime", getUptime().c_str());
    
    mvwprintw(win, 4, 1, "%-*s %s%% %s", text_width, "CPU", formatPercentage(cpu_usage).c_str(), usageBar(terminal_width, cpu_usage).c_str());
    mvwprintw(win, 5, 1, "%-*s %05.2f  C", text_width, "Temperature", cpu.getTemperature());
    mvwprintw(win, 6, 1, "%-*s %05.2f  GHz", text_width, "Clock", cpu.getClockSpeed());
    
    mvwprintw(win, 8, 1, "%-*s %s%% %s", text_width, "Memory", formatPercentage(mem_usage).c_str(), usageBar(terminal_width, mem_usage).c_str());
    
    mvwprintw(win, 10, 1, "%-*s %s%% %s", text_width, "Disk", formatPercentage(disk_usage).c_str(), usageBar(terminal_width, disk_usage).c_str());

    mvwprintw(win, 12, 1, "%-*s %s %.2f %s %.2f", text_width, "Network", "Download", net_stats.download_speed, "Upload", net_stats.upload_speed);
    
    mvwprintw(win, 14, 1, "%-*s %s %d  %s %d  %s %d", text_width, "Process", "Total", proc_info.total, "Running", proc_info.running, "Sleeping", proc_info.sleeping);  
    mvwprintw(win, 15, 1, "%-*s %s %.2f  %s %.2f  %s %.2f", text_width, "Load Avg", "1m", load_avg.one_min, "5m", load_avg.five_min, "15m", load_avg.fifteen_min); 
    
    mvwprintw(win, 17, 1, "%-*s %-6s %-10s %-9s", text_width, "High Usage", "PID", "Command", "CPU Usage");
    for (int i = 0; i < 5 && i < top_processes.size(); i++) {
        mvwprintw(win, 18 + i, 1, "%-*s %-6d %-10s %-6.2f%%", text_width, "", top_processes[i].pid, top_processes[i].command.c_str(), top_processes[i].usage);
    }

    wrefresh(win);
}

void runOverview() {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    WINDOW* win = newwin(max_y, max_x, 0, 0);

    while (true) {
        getmaxyx(stdscr, max_y, max_x);
        wresize(win, max_y, max_x);

        displayOverview(win);
        if (getch() == 'q') {
            break;
        }
        sleep(0.2);
    }
    delwin(win);
}
