// module/disk.cpp
#include <sys/statvfs.h>
#include <string>
#include "disk.hpp"

using namespace std;

DiskSpace getDiskUsage(const string& path) {
    struct statvfs stat;

    if (statvfs(path.c_str(), &stat) != 0) {
        return {0, 0, 0, -1.0f};
    }

    unsigned long total = stat.f_blocks * stat.f_frsize;
    unsigned long available = stat.f_bavail * stat.f_frsize;
    unsigned long used = total - available;
    float usage_percentage = (float)used / total * 100.0f;

    return {total, available, used, usage_percentage};
}
