#include <sys/statvfs.h>
#include <string>
#include "disk.hpp"

using namespace std;

float getDiskUsage(const string& path){
    struct statvfs stat;
    
    if(statvfs(path.c_str(), &stat) != 0){
        return -1.0f;
    }
    
    unsigned long total = stat.f_blocks * stat.f_frsize;
    unsigned long available = stat.f_bavail * stat.f_frsize;
    unsigned long used = total - available;
    float usage = (float)used / total * 100.0f;

    return usage;
}
