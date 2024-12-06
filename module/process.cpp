#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

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

ProcessCount getProcessCount() {
    ProcessCount process_count;
    
    process_count.total = runCommand("ps -e --no-headers | wc -l");
    process_count.running = runCommand("ps -e -o state --no-headers | grep -c \"R\"");
    process_count.sleeping = process_count.total - process_count.running;

    return process_count;
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

vector<ProcessInfo> getTopProcesses() {
    vector<ProcessInfo> processes;
    FILE* pipe = popen("ps -eo pid,comm,pcpu --sort=-pcpu --no-headers | head -n 5", "r");
    if (!pipe) return processes;

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        istringstream iss(buffer);
        ProcessInfo proc;
        iss >> proc.pid >> proc.command >> proc.usage;
        processes.push_back(proc);
    }

    pclose(pipe);
    return processes;
}

vector<ProcessDetails> getTopProcessesDetails() {
    vector<ProcessDetails> processes;
    const char* cmd = "ps -eo pid,pgid,comm,%cpu,%mem,state,nlwp,user,lstart --sort=-%cpu | head -n 30";

    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        cerr << "Error: Unable to run the ps command" << endl;
        return processes;
    }

    char buffer[256];
    bool skip_header = true;

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        if (skip_header) {
            skip_header = false;
            continue;
        }

        istringstream line(buffer);
        ProcessDetails process;

        line >> process.pid
             >> process.pgid
             >> process.command
             >> process.cpu_usage
             >> process.memory_usage
             >> process.state
             >> process.threads
             >> process.user;

        getline(line, process.start_time);

        processes.push_back(process);
    }

    pclose(pipe);
    return processes;
}

