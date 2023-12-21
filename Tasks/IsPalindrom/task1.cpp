#include <iostream>
#include <string>

bool IsPalindrom (std::string& str) {
    return str == std::string(str.rbegin(), str.rend());
}