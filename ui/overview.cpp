#include <ncurses.h>
#include <cstdio>
#include <unistd.h>
#include "cpu.hpp"


void displayOverview(WINDOW* win) {
    CPU cpu;
    werase(win);
    cpu.getUsage();
    usleep(100000);
    float cpu_usage = cpu.getUsage();

    mvwprintw(win, 1, 1, "CPU Usage: %s", getCPUUsageBar(getmaxx(win), cpu_usage).c_str());
    mvwprintw(win, 3, 1, "CPU Usage: %.2f%%", cpu_usage);
    mvwprintw(win, 4, 1, "Temp: %.2f°C", cpu.getTemperature());
    mvwprintw(win, 5, 1, "Clock: %.2fGHz", cpu.getClockSpeed());
    wrefresh(win);
}

void runOverview() {
    WINDOW* win = newwin(10, 50, 0, 0);

    while (true) {
        displayOverview(win);
        if (getch() == 'q') {
            break;
        }
        sleep(1);
    }

    delwin(win);
}
