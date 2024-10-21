#ifndef CPU_HPP
#define CPU_HPP
#include <string>

using namespace std;

string getCPUUsageBar(int thermal_width, float usage);

class CPU {
public:
    float getUsage();
    float getTemperature();
    float getClockSpeed();
};

#endif
