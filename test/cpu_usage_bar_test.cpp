#include <iostream>
#include <unistd.h>
#include "../module/cpu.hpp"

using namespace std;

int main() {
    CPU cpu;

    cpu.getUsage();
	usleep(100000);

    float usage = cpu.getUsage();

    int terminal_width = 80;

    string cpu_bar = getCPUUsageBar(terminal_width, usage);

    cout << "Terminal width: " << terminal_width << endl;
    cout << "CPU Usage: " << usage << "%" << endl;
    cout << "CPU Usage Bar: " << cpu_bar << endl;
}
