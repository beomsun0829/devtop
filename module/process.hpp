#ifndef PROCESS_HPP
#define PROCESS_HPP

struct ProcessInfo{
    int total;
    int running;
    int sleeping;
};

ProcessInfo getProcessInfo();

#endif
