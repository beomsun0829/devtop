#include <iostream>
#include <fstream>

using namespace std;

string usageBar(int terminal_width, float usage){
    int bar_length = terminal_width / 3;
    bar_length = bar_length > 0 ? bar_length : 0;
    
    int filled_length = static_cast<int>(usage * bar_length / 100.0);

    string bar = "[" + string(filled_length, '#') + string(bar_length - filled_length, ' ') + "]";
    return bar;
}

string formatPercentage(float value) {
    char buffer[10];
    if (value < 100) {
        snprintf(buffer, sizeof(buffer), "%05.2f", value);
    } else {
        snprintf(buffer, sizeof(buffer), "%.2f", value);
    }
    return string(buffer);
}
