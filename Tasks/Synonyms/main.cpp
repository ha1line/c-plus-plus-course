#include <map>
#include <string>
#include <iostream>
#include <set>


int main() {
	std::map<std::string, std::set<std::string>> synonyms_dict;
	int N;
	std::string action;
	std::string first_word;
	std::string second_word;
	std::cin >> N;

	for (size_t i = 0; i < N; ++i) {
		std::cin >> action;


		if (action == "ADD") {
			std::cin >> first_word;
			std::cin >> second_word;

			synonyms_dict[first_word].insert(second_word);

			synonyms_dict[second_word].insert(first_word);

		} else if (action == "COUNT") {
			std::cin >> first_word;
			std::cout << synonyms_dict[first_word].size() << std::endl;

		} else if (action == "CHECK") {
			std::cin >> first_word;
			std::cin >> second_word;

			if (synonyms_dict.contains(second_word) &&
				synonyms_dict[second_word].contains(first_word)) {
				std::cout << "YES" << std::endl;
			} else {
				std::cout << "NO" << std::endl;
			}
		}
	}
	return 0;
}
