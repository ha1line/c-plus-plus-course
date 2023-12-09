#include<iostream>
#include<string>


bool IsPalindrom(std::string word) {
    return word == std::string(word.rbegin(), word.rend());
}