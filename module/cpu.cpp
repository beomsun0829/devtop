#include "cpu.hpp"
#include <fstream>
#include <string>
#include <unistd.h>

float CPU::getUsage() {
    std::ifstream file("/proc/stat");
    std::string line;
    std::getline(file, line);

    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
    sscanf(line.c_str(), "cpu  %llu %llu %llu %llu %llu %llu %llu %llu",
           &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);

    static unsigned long long prev_idle = 0, prev_total = 0;
    unsigned long long idle_time = idle + iowait;
    unsigned long long total_time = user + nice + system + idle_time + irq + softirq + steal;

    unsigned long long delta_idle = idle_time - prev_idle;
    unsigned long long delta_total = total_time - prev_total;

    prev_idle = idle_time;
    prev_total = total_time;

    return (1.0f - static_cast<float>(delta_idle) / static_cast<float>(delta_total)) * 100.0f;
}

float CPU::getTemperature() {
    std::ifstream file("/sys/class/thermal/thermal_zone0/temp");
    int temp_milli;
    file >> temp_milli;
    return temp_milli / 1000.0;
}

float CPU::getClockSpeed() {
    std::ifstream file("/proc/cpuinfo");
    std::string line;

    while (std::getline(file, line)) {
        if (line.find("cpu MHz") != std::string::npos) {
            float clock_speed;
            sscanf(line.c_str(), "cpu MHz : %f", &clock_speed);
            return clock_speed / 1000.0f;  // Convert MHz to GHz
        }
    }

    return 0.0f;
}
