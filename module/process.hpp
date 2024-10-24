#ifndef PROCESS_HPP
#define PROCESS_HPP

struct ProcessInfo{
    int total;
    int running;
    int sleeping;
};

struct LoadAvg {
    float one_min;
    float five_min;
    float fifteen_min;
};

ProcessInfo getProcessInfo();
LoadAvg getLoadAvg();

#endif
