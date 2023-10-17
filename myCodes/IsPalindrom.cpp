#include <iostream>
#include <string>

bool IsPalindrom(std::string string){
	std::string reversedString;

	for (int n = string.length() - 1; n >= 0; --n)
	{
		reversedString+=string[n];
	}
	std::string a = "a";
	return string == reversedString;
}
