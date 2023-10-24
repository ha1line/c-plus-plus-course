#include <iostream>
#include <string>
#include <map>
#include <set>

int main()
{
	std::map<std::string, std::set<std::string>> Synonyms {};
    int n;
    std::cin>>n;
    for(int i = 0; i<n;i++)
    {
        std::string nameCommand;
        std::cin>>nameCommand;
        if (nameCommand=="ADD")
        {
            std::string addWord1,addWord2;
            std::cin>>addWord1;
            std::cin>>addWord2;
            Synonyms[addWord1].insert(addWord2);
    		Synonyms[addWord2].insert(addWord1);

        }
        if (nameCommand=="COUNT")
        {
            std::string word;
            std::cin>>word;
            std::cout<<Synonyms[word].size()<<std::endl;
        }
        if (nameCommand=="CHECK")
        {
            std::string checkWord1,checkWord2;
            std::cin>>checkWord1;
            std::cin>>checkWord2;
            
    		if (Synonyms[checkWord1].count(checkWord2)==1)
    		{
        		std::cout<<"YES"<<std::endl;
    		}
   		 	else
    		{
        		std::cout<<"NO"<<std::endl;
    		}
        }
    }
}
