#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <string>
using namespace std;

struct NetworkStats {
    float download_speed;
    float upload_speed;
};

NetworkStats getNetworkUsage(const string& interface);
bool isInterfaceActive(const string& interface);
NetworkStats getMainNetworkUsage(string& active_interface);

#endif
