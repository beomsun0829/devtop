#include "system.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

string getUptime() {
    ifstream uptimeFile("/proc/uptime");
    double uptime_seconds;
    
    if (uptimeFile.is_open()) {
        uptimeFile >> uptime_seconds;
        uptimeFile.close();
    }

    int hours = static_cast<int>(uptime_seconds) / 3600;
    int minutes = (static_cast<int>(uptime_seconds) % 3600) / 60;
    int seconds = static_cast<int>(uptime_seconds) % 60;

    stringstream ss;
    ss << setw(2) << setfill('0') << hours << ":"
       << setw(2) << setfill('0') << minutes << ":"
       << setw(2) << setfill('0') << seconds;

    return ss.str();
}
