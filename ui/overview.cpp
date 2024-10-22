#include <ncurses.h>
#include <cstdio>
#include <unistd.h>

#include "cpu.hpp"
#include "memory.hpp"
#include "format.hpp"

void displayOverview(WINDOW* win) {
    CPU cpu;
    werase(win);
    int terminal_width = getmaxx(win);

    cpu.getUsage();
    usleep(100000);

    float cpu_usage = cpu.getUsage();
    float mem_usage = getMemoryUsage();
    
    int text_width = 8;
    int value_width = 6;

    mvwprintw(win, 1, 1, "%-*s %*.2f%% %s", text_width, "CPU", value_width, cpu_usage, usageBar(terminal_width, cpu_usage).c_str());
    mvwprintw(win, 2, 1, "%-*s %*.2f  C", text_width, "Temp", value_width, cpu.getTemperature());
    mvwprintw(win, 3, 1, "%-*s %*.2f  GHz", text_width, "Clock", value_width, cpu.getClockSpeed());
    mvwprintw(win, 5, 1, "%-*s %*.2f%% %s", text_width, "Mem", value_width, mem_usage, usageBar(terminal_width, mem_usage).c_str());

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
