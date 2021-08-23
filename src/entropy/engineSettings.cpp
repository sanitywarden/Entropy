#include "engineSettings.hpp"

entropy::Settings::Settings() {
    this->window_size = sf::Vector2f(800, 600);
    this->window_fullscreen = false;
}

entropy::Settings::~Settings() {

}

// Extract part of the string.
// Argument from is inclusive.
// Argument to is exclusive.
const std::string entropy::applicationSettings::extractPartString(const int& from, const int& to, const std::string& str) {
    std::string value;

    for(int i = from; i < to; i++) {
        value += str[i];
    }

    return value;
}

entropy::applicationSettings::applicationSettings() {
    this->m_settings = entropy::Settings();

    this->loadUserSettings();
}

entropy::applicationSettings::~applicationSettings() {
    this->saveUserSettings();
}

void entropy::applicationSettings::loadUserSettings() {
    std::string filename = "./config/application_settings.ege";
    
    // Check if the file has the correct extension.
    int length = filename.length();
    if(filename[length - 1] != 'e' && filename[length - 2] != 'g' && filename[length - 3] != 'e' && filename[length - 4] != '.') {
        filename += ".ege";
    }

    // Check if the file has been opened correctly, or if the file even exists.
    // If the file could not be opened, use default settings instead.
    std::fstream input_file(filename.c_str(), std::ios::in);
    if(!input_file) {
        this->m_settings = entropy::Settings();
        
        std::cout << "[Entropy Engine][Engine Settings]: Custom user settings file not found.\n";
        std::cout << "[Entropy Engine][Engine Settings]: Application loaded with default settings.\n";        
        
        return;
    } 

    std::cout << "[Entropy Engine][Engine Settings]: Custom user settings file loaded.\n";

    std::string file_line;

    // Load the settings...
    // Extract the window size from the file.
    std::getline(input_file, file_line);

    int index = file_line.find_first_of('x');
    auto width  = std::stoi(this->extractPartString(0, index, file_line));
    auto height = std::stoi(this->extractPartString(index + 1, file_line.length(), file_line));

    this->m_settings.window_size = sf::Vector2f(width, height);
    
    std::cout << "[Entropy Engine][Engine Settings]: Window size:\t" << width << "x" << height << "\n";

    // Extract if the window is supposed to be fullscreen.
    std::getline(input_file, file_line);

    int is_fullscreen = std::stoi(file_line);
    this->m_settings.window_fullscreen = is_fullscreen;
    std::string prompt = (is_fullscreen) ? "True" : "False";
    std::cout << "[Entropy Engine][Engine Settings]: Fullscreen:\t" << prompt << "\n";

    // Close the file.

    input_file.close();
}

void entropy::applicationSettings::saveUserSettings() {
    std::string filename = "./config/application_settings.ege";

    // Check if the file has the correct extension.
    int length = filename.length();
    if(filename[length - 1] != 'e' && filename[length - 2] != 'g' && filename[length - 3] != 'e' && filename[length - 4] != '.') {
        filename += ".ege";
    }

    // Check if the file has been opened correctly, or if the file even exists.
    // If the file could not be opened, do not save the settings.
    std::fstream output_file(filename.c_str(), std::ios::out);
    if(!output_file) {
        std::cout << "[Entropy Engine][Engine Settings]: User settings config file not found. Settings will not be saved.\n";
    
        return;
    }

    std::cout << "[Entropy Engine][Engine Settings]: Attempting to save settings.\n";

    // Save the settings...
    output_file << this->m_settings.window_size.x << "x" << this->m_settings.window_size.y << "\n";
    output_file << this->m_settings.window_fullscreen << "\n";

    // Close the file.

    output_file.close();
}

const entropy::Settings& entropy::applicationSettings::getUserSettings() {
    return this->m_settings;
}