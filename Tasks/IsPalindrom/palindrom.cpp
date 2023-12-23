#include <iostream>
#include <string>

bool IsPalindrome(std::string str) {
    int left = 0;               
    int right = str.length() - 1;   

    while (left < right) {
        if (str[left] != str[right]) {
            return false;  
        }

        left++;        
        right--;           
    }

    return true;  
}

int main() {
    std::string str;
    std::cin >> str;

    std::cout << IsPalindrome(str) << std::endl;

    return 0;
}
