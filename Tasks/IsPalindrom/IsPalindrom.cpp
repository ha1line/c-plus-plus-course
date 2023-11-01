#include <string>
#include <algorithm>

bool IsPalindrom(const std::string &curr_str) {
    auto rev_str(curr_str);
    std::reverse(rev_str.begin(), rev_str.end());
    return rev_str == curr_str;
}




