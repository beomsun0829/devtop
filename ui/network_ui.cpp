#include <ncurses.h>
#include <string>
#include <vector>
#include <sstream>
#include "network.hpp"

using namespace std;

void displayNetwork(WINDOW* win) {
    int terminal_width = getmaxx(win);
    int text_width = 20;

    string active_interface;
    NetworkStats stats = getMainNetworkUsage(active_interface);
    InterfaceDetails details = getInterfaceDetails(active_interface);
    vector<ConnectionDetails> connections = listOpenConnections();
    NetworkErrors errors = getNetworkErrors(active_interface);
    string routing_table = getRoutingTable();
    string public_ip = getPublicIPAddress();
    string isp_info = getISPInfo();

    werase(win);

    mvwprintw(win, 1, 1, "%-*s: %s", text_width, "Active Interface", active_interface.c_str());

    mvwprintw(win, 3, 1, "%-*s: %.2f MB/s", text_width, "Download Speed", stats.download_speed);
    mvwprintw(win, 4, 1, "%-*s: %.2f MB/s", text_width, "Upload Speed", stats.upload_speed);

    mvwprintw(win, 6, 1, "%-*s: %s", text_width, "IPv4 Address", details.ipv4_address.c_str());
    mvwprintw(win, 7, 1, "%-*s: %s", text_width, "IPv6 Address", details.ipv6_address.c_str());
    mvwprintw(win, 8, 1, "%-*s: %s", text_width, "MAC Address", details.mac_address.c_str());
    mvwprintw(win, 9, 1, "%-*s: %s", text_width, "Interface Speed", details.speed.c_str());

    mvwprintw(win, 11, 1, "%-*s: %lu", text_width, "RX Dropped", errors.rx_dropped);
    mvwprintw(win, 12, 1, "%-*s: %lu", text_width, "RX Errors", errors.rx_errors);
    mvwprintw(win, 13, 1, "%-*s: %lu", text_width, "Collisions", errors.rx_collisions);

    mvwprintw(win, 15, 1, "%-*s: %s", text_width, "Public IP", public_ip.c_str());
    mvwprintw(win, 16, 1, "%-*s: %s", text_width, "ISP", isp_info.c_str());

    mvwprintw(win, 18, 1, "Routing Table:");
    
    int y_offset = 19;
    istringstream routing_stream(routing_table);
    string line;
    while (getline(routing_stream, line)) {
        if (y_offset >= getmaxy(win) - 2) break;
        mvwprintw(win, y_offset++, 2, "%s", line.c_str());
    }
    /*
    mvwprintw(win, y_offset++, 1, "Connections:");
    for (const auto& conn : connections) {
        if (y_offset >= getmaxy(win) - 2) break;
        mvwprintw(win, y_offset++, 2, "%s %s -> %s (%s)", conn.protocol.c_str(),
                  conn.local_address.c_str(), conn.remote_address.c_str(), conn.state.c_str());
    }
    */

    wrefresh(win);
}

