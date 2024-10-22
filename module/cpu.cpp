#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <filesystem>
#include "cpu.hpp"
using namespace std;

float CPU::getUsage() {
    ifstream file("/proc/stat");
    string line;
    getline(file, line);

    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
    sscanf(line.c_str(), "cpu  %llu %llu %llu %llu %llu %llu %llu %llu",
           &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);

    static unsigned long long prev_idle = 0, prev_total = 0;
    unsigned long long idle_time = idle + iowait;
    unsigned long long total_time = user + nice + system + idle_time + irq + softirq + steal;

    unsigned long long delta_idle = idle_time - prev_idle;
    unsigned long long delta_total = total_time - prev_total;

    prev_idle = idle_time;
    prev_total = total_time;
	
    if(delta_total == 0){
        return 0.0f;
    }
		
    return (1.0f - static_cast<float>(delta_idle) / static_cast<float>(delta_total)) * 100.0f;
}

float CPU::getTemperature() {
    string base_path = "/sys/class/thermal/";
    string temp_path;

    bool found = false;
    for (const auto& entry : filesystem::directory_iterator(base_path)){
        if (entry.is_directory() && entry.path().filename().string().find("thermal_zone") != string::npos){
            string type_path = entry.path().string() + "/type";
            ifstream typeFile(type_path);

            if (!typeFile.is_open()){
                continue;
            }

            string type;
            getline(typeFile, type);

            if (type == "x86_pkg_temp"){
                temp_path = entry.path().string() + "/temp";
                found = true;
                break;
            }
        }
    }
	
    if (!found){
        return 0.0f;
    }

    ifstream temp_file(temp_path);
    if(!temp_file.is_open()){
        return 0.0f;
    }
    
    int temp_milli;
    temp_file >> temp_milli;
    
    return temp_milli / 1000.0f;
}

float CPU::getClockSpeed() {
    ifstream file("/proc/cpuinfo");
    string line;

    while (getline(file, line)) {
        if (line.find("cpu MHz") != string::npos) {
            float clock_speed;
            sscanf(line.c_str(), "cpu MHz : %f", &clock_speed);
            return clock_speed / 1000.0f;
        }
    }

    return 0.0f;
}
