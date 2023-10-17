#include <iostream>
#include <map>
#include <set>

bool Check(std::map<std::string, std::set<std::string>>& dictionary, const std::string& word1, std::string& word2){
    return dictionary[word1].count(word2)>0;;
}

int main(){
    std::map<std::string, std::set<std::string>> dictionary;
    size_t n;
    std::cin >> n;
    for (int i = 0; i < n; ++i){
        std::string command;
        std::cin >> command;
        if (command == "ADD"){
            std::string word1, word2;
            std::cin >> word1 >> word2;
            dictionary[word1].insert(word2);
            dictionary[word2].insert(word1);
        }
        else if (command=="COUNT"){
            std::string word1;
            std::cin>>word1;
            std::cout<<dictionary[word1].size()<<std::endl;
        }

        else if(command == "CHECK"){
            std::string word1, word2;
            std::cin >> word1 >> word2;
            std::cout << (Check(dictionary, word1, word2) ? "YES" : "NO") << std::endl;
        }
    }
}