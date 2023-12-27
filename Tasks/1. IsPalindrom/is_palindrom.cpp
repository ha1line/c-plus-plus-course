#include <iostream>

bool IsPalindrom(const std::string& string)
{
    int length = string.length();
    for (int i = 0; i < length / 2; i++)
    {
        if (string[i] != string[length - i - 1])
        {
            return false;
        }
    }
    return true;
}