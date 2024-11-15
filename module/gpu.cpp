#include <string>
#include <cstdio>
#include <cstdlib>
#include "gpu.hpp"

using namespace std;

float getGpuUsage() {
    FILE* fp = popen("nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader,nounits", "r");
    if (!fp) return -1.0f;

    int usage;
    fscanf(fp, "%d", &usage);
    pclose(fp);
    return static_cast<float>(usage);
}

float getGpuMemoryUsage() {
    FILE* fp = popen("nvidia-smi --query-gpu=memory.used,memory.total --format=csv,noheader,nounits", "r");
    if (!fp) return -1.0f;

    int used_memory, total_memory;
    fscanf(fp, "%d, %d", &used_memory, &total_memory);
    pclose(fp);
    if (total_memory == 0) return 0.0f;

    return (static_cast<float>(used_memory) / total_memory) * 100.0f;
}

float getGpuTemperature() {
    FILE* fp = popen("nvidia-smi --query-gpu=temperature.gpu --format=csv,noheader,nounits", "r");
    if (!fp) return -1.0f;

    int temperature;
    fscanf(fp, "%d", &temperature);
    pclose(fp);
    return static_cast<float>(temperature);
}

