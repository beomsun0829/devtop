#include <fstream>
#include <string>
#include <cstdio>
#include "memory.hpp"

using namespace std;

float getMemoryUsage() {
    ifstream meminfo("/proc/meminfo");

    string key;
    long total_mem = 0, free_mem = 0, available_mem = 0;

    while (meminfo >> key) {
        if (key == "MemTotal:") {
            meminfo >> total_mem;
        } else if (key == "MemAvailable:") {
            meminfo >> available_mem;
            break;
        }
    }

    if (total_mem == 0) {
        return 0.0f;
    }

    float used_mem = total_mem - available_mem;
    return (used_mem / total_mem) * 100.0f;
}
