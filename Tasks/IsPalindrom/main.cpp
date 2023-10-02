#include <iostream>
#include <string>
bool IsPalindrom(const std::string word);

int main()
{
    std::string word;
    std::cin >> word;
    bool a = IsPalindrom(word);
    std::cout << a << std::endl;
}
bool IsPalindrom(const std::string word)
{
    for (int i = 0, j = word.size() - 1; i < word.size(); ++i, --j)
    {
        if (word[i] != word[j])
        {
            return false;
        }   
    }
    return true;
}
