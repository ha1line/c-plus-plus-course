#include <iostream>
#include <map>
#include <string>
#include <set>

std::map<std::string, std::string> synonyms;
std::map<std::string, std::set<std::string>> synonyms_list;

void add(std::string word1, std::string word2) {
    synonyms[word1] = word2;
    synonyms[word2] = word1;
    synonyms_list[word1].insert(word2);
    synonyms_list[word2].insert(word1);
}

int count(std::string word) {
    return synonyms_list[word].size();
}

bool check(std::string word1, std::string word2) {
    return synonyms[word1] == word2 || synonyms[word2] == word1;
}

int main() {
    int n;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        std::string command, word1, word2;
        std::cin >> command;
        if (command == "ADD") {
            std::cin >> word1 >> word2;
            add(word1, word2);
        } else if (command == "COUNT") {
            std::cin >> word1;
            std::cout << count(word1) << "\n";
        } else if (command == "CHECK") {
            std::cin >> word1 >> word2;
            std::cout << (check(word1, word2) ? "YES" : "NO") << "\n";
        }
    }
}


