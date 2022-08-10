#include "globalutilities.hpp"
#include "generationSettings.hpp"

/* Do not add 'using namespace iso' here.
 * If the implementation does not have iso namespace then the compiler will not associate the definitions with this implementation. 
 * The code would not compile without specifying the namespace. */

#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <random>

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

bool iso::endsWith(const std::string& str, const std::string& phrase) {
    if(str.length() < phrase.length())
        return false;

    int match = 0;
    for(int i = str.length() - phrase.length(), c = 0; i < str.length(); i++, c++) {
        if(str[i] == phrase[c])
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

std::string iso::readAfter(const std::string& str, const std::string& phrase) {
    if(!str.length())
        return "";
    
    size_t phrase_start = iso::find(str, phrase);
    if(phrase_start == std::string::npos)
        return "";

    if(phrase.length() > 1)
        phrase_start += phrase.length();

    std::string extract_string;
    for(int i = phrase_start; i < str.length(); i++) {
        extract_string.append(1, str[i]);
    }

    return extract_string;
}

std::string iso::readBefore(const std::string& str, const std::string& phrase) {
    // 1 is the length of a single char.
    if(str.length() < 1)
        return "";
    
    const size_t index = str.find(phrase[0]);
    if(index == std::string::npos)
        return "";

    if(!iso::containsWord(str, phrase))
        return "";

    // Because there can be multiple same characters your phrase starts with, you need to check each of these.
    // The index of the current character is saved here. 
    size_t occurence = index;
    int match = 0;

    // While there are characters you are searching for in the string, repeat the process.
    while(str.find(phrase[0], occurence + 1) != std::string::npos) {
        for(int i = occurence; i < str.length(); i++) {
            for(int j = 0; j < phrase.length(); j++) {
                if(str[i] == phrase[j]) {
                    match++;
                }
    
                if(match >= phrase.length())
                    return iso::read(str, 0, occurence);
    
                if(j == phrase.length() - 1)
                    match = 0;
            }
    
        }

        // Because string after the first character did not match the phrased you search for,
        // find the next occurence of the first character.
        occurence = str.find(phrase[0], occurence + 1);
    }

    std::string extract_string;
    for(int i = 0; i < occurence; i++) {
        extract_string.append(1, str[i]);
    }

    return extract_string;
}

std::string iso::read(const std::string& str, int incl_from, int to) {
    std::string extract_string;
    for(int i = incl_from; i < to; i++) {
        extract_string.append(1, str[i]);
    }

    return extract_string;
}

std::string iso::toLower(const std::string& str) {
    std::string str_lower;
    for(int i = 0; i < str.length(); i++) {
        const char character = str.at(i);
        int ascii_value = (int)character;
        
        if(ascii_value >= 65 && ascii_value <= 90)
            ascii_value += 32;

        const char replaced_character = (char)ascii_value;
        str_lower.append(1, replaced_character);
    }

    return str_lower;
}

std::string iso::toHigher(const std::string& str) {
    std::string str_higher;
    for(int i = 0; i < str.length(); i++) {
        const char character = str.at(i);
        int ascii_value = (int)character;

        if(ascii_value >= 97 && ascii_value <= 122)
            ascii_value -= 32;
        
        const char replaced_character = (char)ascii_value;
        str_higher.append(1, replaced_character);
    }

    return str_higher;
}

bool iso::inCircle(const sf::Vector2f point, const sf::Vector2f centre, const int radius) {
    return ((point.x - centre.x) * (point.x - centre.x)) + ((point.y - centre.y) * (point.y - centre.y)) < radius * radius;
}

/* If the phrase is found, returns the index of the first character of the phrase.
 * Example: when searching for "_is" in "this_isoity_is_useful" function will return the position of "_" - index 12.
 */
size_t iso::find(const std::string& str, const std::string& phrase) {
    if(str.length() < phrase.length())
        return std::string::npos;

    if(!iso::containsWord(str, phrase))
        return std::string::npos;

    size_t index = str.find(phrase[0]);
    if(index == std::string::npos)
        return std::string::npos;

    int match = 0;
    size_t occurence = 0;
    for(int i = index; i < str.length(); i++) {
        for(int j = 0; j < phrase.length(); j++) {
            // If found, increment the counter.
            if(str[i] == phrase[j]) {
                match++;
            }

            // Already found that the text exists inside the string.
            if(match >= phrase.length()) {
                occurence = phrase.length() > 1
                    ? i - phrase.length() // You delete the indexes that you passed when matching a word. 
                    : i;                  // If the phrase's length is 1, you do not need to delete the length, because the occurence index is the index of the whole phrase.
                return occurence;
            }

            // If it's the end of the loop, and no characters are matching, reset the counter.
            else if(j == phrase.length() - 1)
                match = 0;
        }
    }

    return occurence;
}

std::string iso::trimWhitespace(const std::string& str) {
    std::string trimmed_string;
    for(int i = 0; i < str.length(); i++) {
        if(str[i] != ' ')
            trimmed_string.append(1, str[i]);
    }

    return trimmed_string;
}

std::string iso::asBool(int number) {
    return number ? "True" : "False";
}

std::string iso::asBool(bool condition) {
    return condition ? "True" : "False";
}

std::string iso::capitalise(const std::string& str) {
    auto lower = iso::toLower(str);
    int ascii_value = (int)lower[0];
    if(ascii_value >= 97 && ascii_value <= 122)
        lower[0] = char(ascii_value - 32);
    return lower;
}

sf::Vector2i iso::tileGridPosition(const sf::Texture& tile_template, sf::Vector2f tile_position) {
    sf::Vector2i cell(
        tile_position.x / game_settings.tileSize().x,
        tile_position.y / game_settings.tileSize().y
    );

    sf::Vector2i selected(
        (cell.y - game_settings.tileOffset().y) + (cell.x - game_settings.tileOffset().x),
        (cell.y - game_settings.tileOffset().y) - (cell.x - game_settings.tileOffset().x)
    );

    sf::Vector2i position_within_tile(
        (int)tile_position.x % (int)game_settings.tileSize().x,
        (int)tile_position.y % (int)game_settings.tileSize().y
    );

    auto colour_name = iso::getTilePixelColour(tile_template, position_within_tile);
    if(colour_name == "Red")
        selected += sf::Vector2i(-1, 0);

    if(colour_name == "Green")
        selected += sf::Vector2i(1, 0);

    if(colour_name == "Blue")
        selected += sf::Vector2i(0, -1);
    
    if(colour_name == "Yellow")
        selected += sf::Vector2i(1, 0);

    return selected;
}

sf::Vector2i iso::tileGridPosition(int index) {
    return sf::Vector2i(
        index % game_settings.getRegionWidth(),
        index / game_settings.getRegionWidth()
    );
}

std::string iso::getTilePixelColour(const sf::Texture& tile_template, sf::Vector2i pixel) {
    if(pixel.x < 0 || pixel.y < 0) 
        return "Other";

    if(pixel.x > game_settings.tileSize().x - 1 || pixel.y > game_settings.tileSize().y - 1) 
        return "Other";

    auto pixel_colour = tile_template.copyToImage().getPixel(pixel.x, pixel.y);

    if(pixel_colour == sf::Color::Red)         return "Red";
    else if(pixel_colour == sf::Color::Green)  return "Green";
    else if(pixel_colour == sf::Color::Blue)   return "Blue";
    else if(pixel_colour == sf::Color::Yellow) return "Yellow";
    else return "Other";
}

int iso::randomInclusiveBetween(int min, int max) {
    int number = std::rand() % max + min;
    return number;
}