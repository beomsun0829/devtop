#include <ncurses.h>
#include "cpu.hpp"
#include <cstdio>
#include <unistd.h>

void displayOverview(WINDOW* win) {
    CPU cpu;
    werase(win);
    mvwprintw(win, 1, 1, "CPU Usage: %.2f%%", cpu.getUsage());
    mvwprintw(win, 2, 1, "Temp: %.2f°C", cpu.getTemperature());
    mvwprintw(win, 3, 1, "Clock: %.2fGHz", cpu.getClockSpeed());
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
