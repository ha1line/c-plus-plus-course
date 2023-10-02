#include <iostream>
#include <string>

bool IsPalindrome(const std::string& str)
{
    int rightBorder = 0;
    for (size_t i = 0; i < str.length()/2; i++)
    {
        if (str[i]!=str[str.length()-1-rightBorder])
        {
            return false;
        }
        rightBorder++;
    }
    return true;
}
