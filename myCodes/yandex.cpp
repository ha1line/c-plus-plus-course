#include <iostream>
#include <vector>
#include <algorithm>
void Make(std::string str){
    std::string newString;
    newString.push_back(str.at(0));
    std::vector<char> nenuzh = {'a', 'e', 'h', 'i', 'o', 'u', 'w', 'y'};
    std::vector<char> num1 = {'b', 'f', 'p', 'v'};
    std::vector<char> num2 = {'c', 'g', 'j', 'k', 'q', 's', 'x', 'z'};
    std::vector<char> num3 = {'d', 't'};
    std::vector<char> num4 = {'l'};
    std::vector<char> num5 = {'m', 'n'};
    std::vector<char> num6 = {'r'};

    for (size_t let = 1; let < str.size(); ++let){
        if(find(nenuzh.begin(), nenuzh.end(), str[let]) != nenuzh.end())
        {
            continue;
        }
        if (find(num1.begin(), num1.end(), str[let]) != num1.end()){
            newString.push_back('1');}
            
        else if(find(num2.begin(), num2.end(), str[let]) != num2.end()){
            newString.push_back('2');}
        else if(find(num3.begin(), num3.end(), str[let]) != num3.end()){
            newString.push_back('3');}
        else if(find(num4.begin(), num4.end(), str[let]) != num4.end()){
            newString.push_back('4');}
        else if(find(num5.begin(), num5.end(), str[let]) != num5.end()){
            newString.push_back('5');}
        else if(find(num6.begin(), num6.end(), str[let]) != num6.end()){
            newString.push_back('6');}
    }

    std::string zalupa;
    zalupa.push_back(newString[0]);
    for (size_t i = 1; i < newString.size(); i++)
    {
        if (newString[i] == newString[i - 1]){
            continue;
        }
        zalupa.push_back(newString[i]);
    }
    if (zalupa.size() > 4)
        zalupa.erase(4);
    else
        for (auto i = zalupa.size(); i < 4; ++i)
            zalupa.push_back('0');
    std::cout<<zalupa<<std::endl;
}


int main(){
    std::string str;
    std::cin >> str;
    Make(str);
}