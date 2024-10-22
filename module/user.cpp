#include "user.hpp"
#include <unistd.h>
#include <string>
using namespace std;

string getCurrentUser() {
    char* user = getlogin();
    if (user != nullptr) {
        return string(user);
    }
    return "Unknown";
}
