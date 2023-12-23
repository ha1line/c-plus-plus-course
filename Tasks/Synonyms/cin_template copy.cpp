#include <iostream>
#include <string>
#include <vector>

class Synonyms_Dictionary{
    public:
        std::vector<std::vector<std::string>> dictionary;
        int find_pos(std::string word){
            for (int i = 0; i < dictionary.size(); ++i){
                for (std::string s : dictionary[i]){
                    if (s == word){
                        return i;
                    }
                }
            }
            return -1;
        }

        bool check(std::string word1, std::string word2){
            int pos1, pos2;
            pos1 = find_pos(word1);
            pos2 = find_pos(word2);

            if (pos1 == -1 || pos2 == -1){
                return false;
            }
            else if (pos1 == pos2){
                return true;
            }
            else{
                return false;
            }
        }

        void add(std::string word1, std::string word2){
            if (check(word1, word2) != true){
                std::vector<std::string> new_syn;
                new_syn.push_back(word1);
                new_syn.push_back(word2);
                dictionary.push_back(new_syn);
            }
        }

        int count(std::string word){
            int count = 0;

            for (int i = 0; i < dictionary.size(); ++i){
                for (std::string s : dictionary[i]){
                    if (s == word){
                        ++count;
                    }
                }
            }

            return count;
        }
};

class Synonyms_Dictionary_transit : Synonyms_Dictionary{
    public:
        std::vector<std::vector<std::string>> dictionary;

        void add(std::string word1, std::string word2){
            int pos1, pos2;
            pos1 = find_pos(word1);
            pos2 = find_pos(word2);

            if (pos1 == -1 && pos2 == -1){
                std::vector<std::string> new_syn;
                new_syn.push_back(word1);
                new_syn.push_back(word2);
                dictionary.push_back(new_syn);
            }
            else if (pos1 != -1 && pos2 == -1){
                dictionary[pos1].push_back(word2);
            }
            else if (pos1 == -1 && pos2 != -1){
                dictionary[pos2].push_back(word1);
            }
        }

        int count(std::string word){
            int pos;
            pos = find_pos(word);

            if (pos == -1){
                return 0;
            }
            else{
                return dictionary[pos].size();
            }
        }
};

int main()
{
    Synonyms_Dictionary dic;
    std::vector<std::string> output;
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
            dic.add(word1, word2);
        }
        else if (command == "COUNT")
        {
            std::string word;
            std::cin >> word;
            output.push_back(std::to_string(dic.count(word)));
        }
        else if (command == "CHECK")
        {
            std::string word1, word2;
            std::cin >> word1 >> word2;
            if (dic.check(word1, word2)){
                output.push_back("YES");
            }
            else{
                output.push_back("NO");
            }
        }
    }

    for (std::string out : output){
        std::cout << out << std::endl;
    }
};
