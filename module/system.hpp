#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <string>
#include <vector>

using namespace std;

string getUptime();
vector<string> getSystemLogs(int num_lines = 0, int log_level = -1, const string& search_keyword = "");

#endif
