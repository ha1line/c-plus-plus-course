
#include <iostream>
#include <string>
#include <map>
#include <set>

using std::string, std::endl, std::cout, std::set, std::map, std::cin;

int main() {
    map<string, set<string>> synonyms;

    string action, word, first_word, second_word;

    int operations_count;
    cin >> operations_count;

    for (int index = 0; index < operations_count; index++) {
        cin >> action;

        if (action == "ADD") {
            cin >> first_word >> second_word;

            synonyms[second_word].insert(first_word);
            synonyms[first_word].insert(second_word);
        } else if (action == "COUNT") {
            cin >> word;
            cout << synonyms[word].size() << endl;
        } else if (action == "CHECK") {
            cin >> first_word >> second_word;

            if (synonyms[first_word].contains(second_word)) {
                cout << "YES" << endl;
            } else {
                cout << "NO" << endl;
            }
        }
    }
}
