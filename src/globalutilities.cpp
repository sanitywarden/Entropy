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

std::string iso::toLower(const std::string& str) {
    std::string str_lower = "";
    for(int i = 0; i < str.length(); i++) {
        const char character          = str.at(i);
        int number_letter_value = (int)character;
        
        if(number_letter_value >= 65 && number_letter_value <= 90)
            number_letter_value += 32;

        const char replaced_character = (char)number_letter_value;
        str_lower.append(1, replaced_character);
    }

    return str_lower;
}

bool iso::inCircle(const sf::Vector2f point, const sf::Vector2f centre, const int radius) {
    return ((point.x - centre.x) * (point.x - centre.x)) + ((point.y - centre.y) * (point.y - centre.y)) < radius * radius;
}