#include <fstream>
#include <string>
#include <unistd.h>
#include <sstream>
#include <vector>
#include "network.hpp"

using namespace std;

NetworkStats getNetworkUsage(const string& interface) {
    ifstream file("/proc/net/dev");
    string line;
    unsigned long rx_bytes = 0, tx_bytes = 0;

    while (getline(file, line)) {
        if (line.find(interface) != string::npos) {
            istringstream iss(line);
            string iface;
            unsigned long temp;
            iss >> iface >> rx_bytes >> temp >> temp >> temp >> temp >> temp >> temp >> temp >> tx_bytes;
            break;
        }
    }

    usleep(1000000);

    file.clear();
    file.seekg(0, ios::beg);

    unsigned long new_rx_bytes = 0, new_tx_bytes = 0;

    while (getline(file, line)) {
        if (line.find(interface) != string::npos) {
            istringstream iss(line);
            string iface;
            unsigned long temp;
            iss >> iface >> new_rx_bytes >> temp >> temp >> temp >> temp >> temp >> temp >> temp >> new_tx_bytes;
            break;
        }
    }

    file.close();

    float download_speed = (new_rx_bytes - rx_bytes) / 1024.0 / 1024.0;
    float upload_speed = (new_tx_bytes - tx_bytes) / 1024.0 / 1024.0;

    return {download_speed, upload_speed};
}

bool isInterfaceActive(const string& interface) {
    ifstream file("/proc/net/dev");
    string line;
    unsigned long rx_bytes = 0, tx_bytes = 0;

    while (getline(file, line)) {
        if (line.find(interface) != string::npos) {
            istringstream iss(line);
            string iface;
            unsigned long temp;
            iss >> iface >> rx_bytes >> temp >> temp >> temp >> temp >> temp >> temp >> temp >> tx_bytes;
            return (rx_bytes > 0 || tx_bytes > 0);
        }
    }
    return false;
}

NetworkStats getMainNetworkUsage(string& active_interface) {
    if (isInterfaceActive("eth0")) {
        active_interface = "eth0";
        return getNetworkUsage("eth0");
    }

    if (isInterfaceActive("wlan0")) {
        active_interface = "wlan0";
        return getNetworkUsage("wlan0");
    }

    active_interface = "None";
    return {0.0f, 0.0f};
}
