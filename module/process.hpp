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

ProcessCount getProcessCount();
LoadAvg getLoadAvg();
vector<ProcessInfo> getTopProcesses();

#endif
