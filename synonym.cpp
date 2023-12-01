#include <iostream>
#include <string>
#include <set>
#include <map>
using set_pair = std::pair<std::string, std::string>;

set_pair sort(std::string& word1, std::string& word2)
{
	return (word1 <= word2) ? make_pair(word1, word2) : make_pair(word2, word1);
}

bool examination(std::set<set_pair>& sinonim, std::string& word1, std::string& word2)
{
	return sinonim.count(sort(word1, word2)) != 0;
}
void insert(std::set<set_pair>& sinonim , std::map<std::string,unsigned int>& sinonim_count ,std::string& word1, std::string& word2)
{
	if (examination(sinonim, word1, word2)) return;
	++sinonim_count[word1];
	if(word1 != word2)
	++sinonim_count[word2];

	sinonim.insert(sort(word1, word2));
}



int main()
{
	std::set<set_pair> sinonim;
	std::map<std::string, unsigned int> sinonim_count;
	std::string request, word , word1, word2;
	int q;
	std::cin >> q;
	for ( int i = 0; i < q; ++i)
	{
		std::cin >> request;
		if (request == "ADD")
		{
			std::cin >> word1 >> word2;
			insert(sinonim, sinonim_count, word1, word2);

		}
		else if (request == "COUNT")
		{
			std::string word;
			std::cin >> word;
			std::cout << sinonim_count[word] << std::endl;
		}
		else if (request == "CHECK")
		{
			std::string word1, word2;
			std::cin >> word1 >> word2;
			if (examination(sinonim, word1, word2))
			{
				std::cout << "YES" << std::endl; 
			}
			else std::cout << "NO" << std::endl;
		}

	}
}
