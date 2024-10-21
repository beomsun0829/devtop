#include <iostream>
#include "../module/cpu.hpp"

int main() {
    CPU cpu;

    float usage = cpu.getUsage();
    std::cout << "CPU Usage: " << usage << "%" << std::endl;

    return 0;
}
