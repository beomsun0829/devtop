#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <string>
#include <vector>

struct NetworkStats {
    float download_speed;
    float upload_speed;
};

struct InterfaceDetails {
    std::string ipv4_address;
    std::string ipv6_address;
    std::string mac_address;
    std::string speed;
};

struct ConnectionDetails {
    std::string protocol;
    std::string local_address;
    std::string remote_address;
    std::string state;
};

struct NetworkErrors {
    unsigned long rx_dropped;
    unsigned long rx_errors;
    unsigned long rx_collisions;
};

struct PingResult {
    std::string result;
};

struct TracerouteResult {
    std::string result;
};

struct VPNStatus {
    std::string status;
};

std::string execCommand(const std::string& cmd);

NetworkStats getNetworkUsage(const std::string& interface);
NetworkStats getMainNetworkUsage(std::string& active_interface);

InterfaceDetails getInterfaceDetails(const std::string& interface);

std::vector<ConnectionDetails> listOpenConnections();

NetworkErrors getNetworkErrors(const std::string& interface);

std::string getRoutingTable();
std::string getFirewallRules();

PingResult pingTest(const std::string& destination);
TracerouteResult traceRoute(const std::string& destination);

VPNStatus getVPNStatus();

std::string getPublicIPAddress();
std::string getISPInfo();

#endif 

