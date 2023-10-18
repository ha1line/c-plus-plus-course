#include <iostream>
#include <unordered_map>
#include <unordered_set>

using dictionary = std::unordered_set<std::string_view>;

int main() {
	std::string_view check_answer[] = {"NO", "YES"};
	std::unordered_map<std::string_view, dictionary> synonyms;
	std::unordered_set<std::string> words;

	size_t actions = 0;
	std::cin >> actions;

	for( ; actions > 0; --actions ) {
		std::string action, word1, word2;
		std::cin >> action;
		if( action == "ADD" ) {
			std::cin >> word1 >> word2;

			std::string_view sw_word1(*(words.emplace(std::move(word1)).first));
			std::string_view sw_word2(*(words.emplace(std::move(word2)).first));

			synonyms[sw_word1].emplace(sw_word2);
			synonyms[sw_word2].emplace(sw_word1);
		} else if( action == "COUNT" ) {
			std::cin >> word1;
			size_t count = 0;
			if( auto it = synonyms.find(word1); it != synonyms.end() ) {
				count = it->second.size();
			}
			std::cout << count << '\n';
		} else if( action == "CHECK" ) {
			bool finded = false;
			std::cin >> word1 >> word2;
			if( auto it = synonyms.find(word1); it != synonyms.end() ) {
				finded = it->second.contains(word2);
			}
			std::cout << check_answer[finded] << '\n';
		}
	}
	return 0;
}