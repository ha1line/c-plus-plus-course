#include <iostream>
#include <map>
#include <set>
#include <vector>

std::map<std::string, std::set<std::string>> dictionary = std::map<std::string, std::set<std::string>>();

void AddCommand(std::string& word1, std::string& word2)
{
    dictionary[word1].insert(word2);
    dictionary[word2].insert(word1);
}

int CountCommand(std::string& word)
{
    return dictionary[word].size();
}

std::string CheckCommand(std::string& word1, std::string& word2)
{
    if (dictionary[word1].find(word2) != dictionary[word1].end())
    {
        return "YES";
    }
    return "NO";
}

int main()
{
    int n;
    std::cin >> n;
    for (int i = 0; i < n; i++)
    {
        std::string command;
        std::cin >> command;
        if(command == "ADD")
        {
            std::string word1, word2;
            std::cin >> word1 >> word2;
            AddCommand(word1, word2);
        }
        else if (command == "COUNT")
        {
            std::string word;
            std::cin >> word;
            std::cout << CountCommand(word) << std::endl;
        }
        else if (command == "CHECK")
        {
            std::string word1, word2;
            std::cin >> word1 >> word2;
            std::cout << CheckCommand(word1, word2) << std::endl;
        }     
    }
}
