#include <iostream>
#include <fstream>

using namespace std;

string usageBar(int terminal_width, float usage){
    int bar_length = terminal_width - 40;
    bar_length = bar_length > 0 ? bar_length : 0;
    
    int filled_length = static_cast<int>(usage * bar_length / 100.0);

    string bar = "[" + string(filled_length, '#') + string(bar_length - filled_length, ' ') + "]";
    return bar;
}
