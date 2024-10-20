#include "cpu.hpp"
#include <cstdio>

void displayOverview() {
    CPU cpu;
    printf("CPU Usage: %.2f%%   Temp: %.2f°C   Clock: %.2fGHz\n", 
           cpu.getUsage(), cpu.getTemperature(), cpu.getClockSpeed());
}
