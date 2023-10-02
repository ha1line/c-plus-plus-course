#include <iostream>
#include <string>

std::string ReverseString(std::string string) {
    auto strLength = string.length();
    std::string reversedStr;

    while (strLength--) {
        reversedStr += string[strLength];
    }

    return reversedStr;
}

bool IsPalindrom(std::string string) {
    return string == ReverseString(string);
}

