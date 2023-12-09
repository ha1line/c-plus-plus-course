#include <iostream>
#include <map>
#include <set>
#include <algorithm>
int main1()
{
    int N = 0;
    std::cin >> N;
    std::map<std::string, std::set<std::string>> container;
    for (int i = 0; i < N; ++i)
    {
        std::string command;
        std::cin >> command;
        if (command == "ADD")
        {
            std::string word1, word2;
            std::cin >> word1 >> word2;
            container[word1].insert(word2);
            container[word2].insert(word1);
        }
        else if (command == "COUNT")
        {
            std::string word;
            int num = 0;
            std::cin >> word;
            for (const auto& [key, value] : container){
                if(key == word){
                    num = value.size();
                }
            }
            std::cout << num << std::endl;
        }
        else if (command == "CHECK")
        {
            std::string word1;
            std::string word2;
            std::cin >> word1 >> word2;
            std::string flag = (container[word1].count(word2) > 0) ? "YES" : "NO";
            std::cout << flag << std::endl;
        }
    }
}
