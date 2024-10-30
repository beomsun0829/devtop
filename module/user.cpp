#include <unistd.h>
#include <string>

#include "user.hpp"

using namespace std;

string getCurrentUser() {
    char* user = getlogin();
    if (user != nullptr) {
        return string(user);
    }
    return "Unknown";
}
