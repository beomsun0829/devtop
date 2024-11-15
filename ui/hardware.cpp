#include <ncurses.h>
#include <string>
#include <vector>
#include "cpu.hpp"
#include "memory.hpp"
#include "disk.hpp"
#include "gpu.hpp"

void displayHardware(WINDOW* win) {
    int terminal_width = getmaxx(win);
    int text_width = 15;

    CPU cpu;
    float cpu_usage = cpu.getUsage();
    float cpu_temperature = cpu.getTemperature();
    float cpu_clock_speed = cpu.getClockSpeed();

    float memory_usage = getMemoryUsage();

    DiskSpace disk_space = getDiskUsage("/");

    float gpu_usage = getGpuUsage();
    float gpu_memory_usage = getGpuMemoryUsage();
    float gpu_temperature = getGpuTemperature();
    bool gpu_available = (gpu_usage >= 0);

    mvwprintw(win, 1, 1, "%-*s: %s%%", text_width, "CPU Usage", std::to_string(cpu_usage).c_str());
    mvwprintw(win, 2, 1, "%-*s: %.2f C", text_width, "CPU Temperature", cpu_temperature);
    mvwprintw(win, 3, 1, "%-*s: %.2f GHz", text_width, "CPU Clock Speed", cpu_clock_speed);

    mvwprintw(win, 5, 1, "%-*s: %s%%", text_width, "Memory Usage", std::to_string(memory_usage).c_str());

    mvwprintw(win, 7, 1, "%-*s: %s%%", text_width, "Disk Usage", std::to_string(disk_space.usage_percentage).c_str());

    if (gpu_available) {
        mvwprintw(win, 9, 1, "%-*s: %s%%", text_width, "GPU Usage", std::to_string(gpu_usage).c_str());
        mvwprintw(win, 10, 1, "%-*s: %s%%", text_width, "GPU Memory Usage", std::to_string(gpu_memory_usage).c_str());
        mvwprintw(win, 11, 1, "%-*s: %.2f C", text_width, "GPU Temperature", gpu_temperature);
    }
    else {
        mvwprintw(win, 9, 1, "%-*s: %s", text_width, "GPU Status", "No GPU Detected");
    }

    wrefresh(win);
}
