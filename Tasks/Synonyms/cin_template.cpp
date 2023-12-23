#include <iostream>
#include <string>
#include <map>
#include <set>

int main()
{
    std::map<std::string, std::set<std::string>> dictionary;
    std::string output = "";

    int N = 0;
    std::cin >> N;
    for (int i = 0; i < N; ++i)
    {
        std::string command;
        std::cin >> command;
        if (command == "ADD")
        {
            std::string word1, word2;
            std::cin >> word1 >> word2;
            
            dictionary[word1].insert(word2);
            dictionary[word2].insert(word1);
        	
        }
        else if (command == "COUNT")
        {
            std::string word;
            std::cin >> word;

            int count = 0;
            
            if (dictionary.find(word) != dictionary.end()){
                count = dictionary[word].size();
            }
            
            output += std::to_string(count) + "\n";
        }
        else if (command == "CHECK")
        {
            std::string word1, word2;
            std::cin >> word1 >> word2;
            
            std::string flag = "NO";
            if (dictionary.find(word1) != dictionary.end()){
                if (dictionary[word1].find(word2) != dictionary[word1].end()){
                    flag = "YES";
                }
            }
        	
            output += flag + "\n";
        }
    }

    std::cout << output;
};
