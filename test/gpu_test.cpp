#include<iostream>
#include "../module/gpu.hpp"

using namespace std;

int main(){
    cout << getGpuUsage() << endl;
    cout << getGpuMemoryUsage() << endl;
    cout << getGpuTemperature()<< endl;
}
