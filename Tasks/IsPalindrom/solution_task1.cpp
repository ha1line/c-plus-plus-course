#include "string"

bool IsPalindrom(const std::string& str) {
    bool result = true;
    int size = str.size();
    for(int i = 0; i<size; i++){
        if(str[i]!=str[size-1-i]){
            result = false;
            break;
        }
    }
    return result;
}