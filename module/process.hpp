#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <vector>
#include <string>

using namespace std;

struct ProcessCount{
    int total;
    int running;
    int sleeping;
};

struct LoadAvg {
    float one_min;
    float five_min;
    float fifteen_min;
};

struct ProcessInfo{
    int pid;
    string command;
    float usage;
};

struct ProcessDetails {
    int pid;
    int pgid;
    string command;
    float cpu_usage;
    float memory_usage;
    string state;
    int threads;
    string user;
    string start_time;
};

ProcessCount getProcessCount();
LoadAvg getLoadAvg();
vector<ProcessInfo> getTopProcesses();
vector<ProcessDetails> getTopProcessesDetails();

#endif
