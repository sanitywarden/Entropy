#include "globalutilities.hpp"

/* Do not add 'using namespace iso' here.
 * If the implementation does not have iso namespace then the compiler will not associate the definitions with this implementation. 
 * The code would not compile without specifying the namespace. */

bool iso::startsWith(const std::string& str, const std::string& phrase) {
    if(str.length() < phrase.length())
        return false;
    
    int match = 0;
    for(int i = 0; i < phrase.length(); i++) {
        if(str[i] == phrase[i])
            match++;
    }

    return match == phrase.length();
}

bool iso::containsWord(const std::string& str, const std::string& phrase) {
    if(str.length() < phrase.length())
        return false;

    const int index = str.find(phrase[0]);
    if(index == std::string::npos)
        return false;

    int match = 0;
    for(int i = index; i < str.length(); i++) {
        for(int j = 0; j < phrase.length(); j++) {
            // If found, increment the counter.
            if(str[i] == phrase[j]) {
                match++;
                break;
            }

            // Already found that the text exists inside the string.
            if(match >= phrase.length())
                return true;

            // If it's the end of the loop, and no characters are matching, reset the counter.
            else if(j == phrase.length() - 1)
                match = 0;
        }
    }    

    return match == phrase.length();
}