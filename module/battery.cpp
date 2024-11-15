#include <fstream>
#include <string>
#include <filesystem>
#include "battery.hpp"

using namespace std;
namespace fs = std::filesystem;

float getBatteryPercentage() {
    string path = "/sys/class/power_supply/BAT0/capacity";
    ifstream file(path);
    if (!file.is_open()) return -1.0f;

    int percentage;
    file >> percentage;
    return static_cast<float>(percentage);
}

string getBatteryStatus() {
    string path = "/sys/class/power_supply/BAT0/status";
    ifstream file(path);
    if (!file.is_open()) return "Unknown";

    string status;
    file >> status;
    return status;
}

int getBatteryTimeRemaining() {
    string path = "/sys/class/power_supply/BAT0/charge_now";
    ifstream file_charge(path);
    path = "/sys/class/power_supply/BAT0/current_now";
    ifstream file_current(path);

    if (!file_charge.is_open() || !file_current.is_open()) return -1;

    int charge_now, current_now;
    file_charge >> charge_now;
    file_current >> current_now;

    if (current_now == 0) return -1;

    return charge_now / current_now;
}
