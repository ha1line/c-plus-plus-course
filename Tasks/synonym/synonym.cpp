#include <iostream>
#include <map>
#include <string>
#include <set>

void add(std::map<std::string, std::set<std::string>>& synonyms) {
    std::string word1, word2;
    std::cin >> word1 >> word2;

    synonyms[word1].insert(word2);
    synonyms[word2].insert(word1);
}

void count(std::map<std::string, std::set<std::string>>& synonyms) {
    std::string word;
    std::cin >> word;

    std::cout << synonyms[word].size() << std::endl;
}

bool check(std::map<std::string, std::set<std::string>>& synonyms) {
    std::string word1, word2;
    std::cin >> word1 >> word2;

    return synonyms[word1].count(word2) > 0;
}

int main() {
    int requestCount;
    std::cin >> requestCount;

    std::map<std::string, std::set<std::string>> synonyms;

    for (int i = 0; i < requestCount; ++i) {
        std::string request;
        std::cin >> request;

        if (request == "ADD")
            add(synonyms);
        
        else if (request == "COUNT")
            count(synonyms);

        else if (request == "CHECK")
            std::cout << (check(synonyms) ? "YES" : "NO") << std::endl;       
    }
}