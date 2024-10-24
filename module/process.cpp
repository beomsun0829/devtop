#include <cstdio>
#include <string>
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
