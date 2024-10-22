#include <ncurses.h>
#include <cstdio>
#include <unistd.h>

#include "cpu.hpp"
#include "memory.hpp"
#include "format.hpp"
#include "user.hpp"
#include "system.hpp"

void displayOverview(WINDOW* win) {
    CPU cpu;
    werase(win);

    cpu.getUsage();
    usleep(100000);

    float cpu_usage = cpu.getUsage();
    float mem_usage = getMemoryUsage();

    int terminal_width = getmaxx(win);    
    int text_width = 12;

    mvwprintw(win, 1, 1, "%-*s %s", text_width, "User", getCurrentUser().c_str());
    mvwprintw(win, 2, 1, "%-*s %s", text_width, "Uptime", getUptime().c_str());
    mvwprintw(win, 4, 1, "%-*s %s%%   %s", text_width, "CPU", formatPercentage(cpu_usage).c_str(), usageBar(terminal_width, cpu_usage).c_str());
    mvwprintw(win, 5, 1, "%-*s %05.2f  C", text_width, "Temperature", cpu.getTemperature());
    mvwprintw(win, 6, 1, "%-*s %05.2f  GHz", text_width, "Clock", cpu.getClockSpeed());
    mvwprintw(win, 8, 1, "%-*s %s%%   %s", text_width, "Memory", formatPercentage(mem_usage).c_str(), usageBar(terminal_width, mem_usage).c_str());

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
        sleep(1);
    }
    delwin(win);
}
