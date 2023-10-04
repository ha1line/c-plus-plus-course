#include <string>
bool IsPalindrom(std::string & input) {
	size_t size = input.size();
	for( size_t i = 0, j = size - 1; i < size/2; ++i, --j ) {
		if( input[i] != input[j] ){
			return false;
		}
	}
	return true;
}