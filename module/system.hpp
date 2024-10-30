#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <string>
#include <vector>

using namespace std;

string getUptime();
vector<string> getSystemLogs(int num_lines = 0);

#endif
