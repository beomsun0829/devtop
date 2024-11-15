#ifndef DISK_HPP
#define DISK_HPP

#include <string>

struct DiskSpace {
    unsigned long total;
    unsigned long available;
    unsigned long used;
    float usage_percentage;
};

DiskSpace getDiskUsage(const std::string& path);

#endif
