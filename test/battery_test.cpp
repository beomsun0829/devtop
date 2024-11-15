#include <iostream>
#include "../module/battery.hpp"

using namespace std;

int main(){
    printf("getBatteryPercentage : %f\n", getBatteryPercentage());
    printf("getBatteryStatus : %s\n", getBatteryStatus());
    printf("getBatteryTimeRemaining : %d\n", getBatteryTimeRemaining());
} 
