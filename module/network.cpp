#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <map>
#include <iostream>
#include <iomanip>
#include <array>
#include <cstring>
#include "network.hpp"

using namespace std;

string execCommand(const string& cmd) {
    array<char, 128> buffer;
    string result;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "Error";
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    pclose(pipe);
    return result;
}

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

InterfaceDetails getInterfaceDetails(const string& interface) {
    InterfaceDetails details;
    struct ifaddrs* ifAddrStruct = nullptr;
    getifaddrs(&ifAddrStruct);

    while (ifAddrStruct != nullptr) {
        if (ifAddrStruct->ifa_name && string(ifAddrStruct->ifa_name) == interface) {
            if (ifAddrStruct->ifa_addr->sa_family == AF_INET) { // IPv4
                details.ipv4_address = inet_ntoa(((struct sockaddr_in*)ifAddrStruct->ifa_addr)->sin_addr);
            } else if (ifAddrStruct->ifa_addr->sa_family == AF_INET6) { // IPv6
                char addr[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, &((struct sockaddr_in6*)ifAddrStruct->ifa_addr)->sin6_addr, addr, sizeof(addr));
                details.ipv6_address = addr;
            }
        }
        ifAddrStruct = ifAddrStruct->ifa_next;
    }
    freeifaddrs(ifAddrStruct);

    struct ifreq ifr;
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    strcpy(ifr.ifr_name, interface.c_str());
    if (ioctl(fd, SIOCGIFHWADDR, &ifr) >= 0) {
        char mac[18];
        snprintf(mac, sizeof(mac), "%02x:%02x:%02x:%02x:%02x:%02x",
                 (unsigned char)ifr.ifr_hwaddr.sa_data[0], (unsigned char)ifr.ifr_hwaddr.sa_data[1],
                 (unsigned char)ifr.ifr_hwaddr.sa_data[2], (unsigned char)ifr.ifr_hwaddr.sa_data[3],
                 (unsigned char)ifr.ifr_hwaddr.sa_data[4], (unsigned char)ifr.ifr_hwaddr.sa_data[5]);
        details.mac_address = mac;
    }
    close(fd);

    details.speed = execCommand("ethtool " + interface + " | grep -i speed");

    return details;
}

vector<ConnectionDetails> listOpenConnections() {
    vector<ConnectionDetails> connections;
    string cmd = "netstat -tun | tail -n +3";
    istringstream iss(execCommand(cmd));
    string line;

    while (getline(iss, line)) {
        istringstream linestream(line);
        string proto, local_addr, remote_addr, state;
        linestream >> proto >> local_addr >> remote_addr >> state;
        if (!proto.empty()) {
            connections.push_back({proto, local_addr, remote_addr, state});
        }
    }
    return connections;
}

NetworkErrors getNetworkErrors(const string& interface) {
    ifstream file("/proc/net/dev");
    string line;
    NetworkErrors errors;

    while (getline(file, line)) {
        if (line.find(interface) != string::npos) {
            istringstream iss(line);
            string iface;
            unsigned long temp;
            iss >> iface >> temp >> errors.rx_dropped >> errors.rx_errors >> errors.rx_collisions;
            break;
        }
    }
    return errors;
}

string getRoutingTable() {
    return execCommand("netstat -r");
}

string getFirewallRules() {
    return execCommand("iptables -L");
}

PingResult pingTest(const string& destination) {
    string result = execCommand("ping -c 4 " + destination);
    return {result};
}

TracerouteResult traceRoute(const string& destination) {
    string result = execCommand("traceroute " + destination);
    return {result};
}

VPNStatus getVPNStatus() {
    string result = execCommand("nmcli connection show --active | grep vpn");
    return {result.empty() ? "No active VPN" : result};
}

string getPublicIPAddress() {
    return execCommand("curl -s https://ipinfo.io/ip");
}

string getISPInfo() {
    return execCommand("curl -s https://ipinfo.io/org");
}

