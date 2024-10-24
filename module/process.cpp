#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "process.hpp"
using namespace std;

int runCommand(const char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return 0;

    char buffer[128];
    string result = "";

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);
    return stoi(result);
}

ProcessInfo getProcessInfo() {
    ProcessInfo info;
    
    info.total = runCommand("ps -e --no-headers | wc -l");
    info.running = runCommand("ps -e -o state --no-headers | grep -c \"R\"");
    info.sleeping = info.total - info.running;

    return info;
}

LoadAvg getLoadAvg(){
    ifstream file("/proc/loadavg");
    string line;
    LoadAvg load_avg = {0.0, 0.0, 0.0};

    if (getline(file, line)) {
        istringstream iss(line);
        iss >> load_avg.one_min >> load_avg.five_min >> load_avg.fifteen_min;
    }

    file.close();
    return load_avg;
}
