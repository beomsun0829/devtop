#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <cstdio>

#include "system.hpp"

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

vector<string> getSystemLogs(int num_lines, int log_level, const string& search_keyword) {
    string cmd = "journalctl";

    if (log_level >= 0) {
        cmd += " -p " + to_string(log_level);
    }
    if (num_lines > 0) {
        cmd += " -n " + to_string(num_lines);
    }
    if (!search_keyword.empty()) {
        cmd += " | grep -i '" + search_keyword + "'";
    }

    vector<string> result;
    char buffer[128];
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return {};

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result.push_back(string(buffer));
    }

    pclose(pipe);
    return result;
}
