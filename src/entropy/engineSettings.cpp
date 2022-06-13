#include "engineSettings.hpp"
#include "globalutilities.hpp"

#include <iostream>

using namespace iso;

Settings::Settings() {
    this->window_size            = sf::Vector2f(1200, 800);
    this->window_fullscreen      = false;
    this->window_vsync           = false;
    this->window_refresh_rate    = 144;
    this->application_debug_mode = true;
}

Settings::~Settings() {

}

applicationSettings::applicationSettings() {
    this->m_settings = Settings();
    this->loadUserSettings();
}

applicationSettings::~applicationSettings() {
}

void applicationSettings::loadUserSettings() {
    std::string filename = "./config/config.config";
    
    // Check if the file has been opened correctly, or if the file even exists.
    // If the file could not be opened, use default settings instead.
    std::fstream input_file(filename.c_str(), std::ios::in);
    if(!input_file) {
        this->m_settings = Settings();
        std::cout << "[Entropy Engine][Engine Settings]: Custom user settings file not found.\n";
        std::cout << "[Entropy Engine][Engine Settings]: Application loaded with default settings.\n";        
        return;
    }

    std::cout << "[Entropy Engine][Engine Settings]: Custom user settings file loaded.\n";

    std::string line_delimiter  = ",";   // What char marks that a line ends.
    std::string read_value_from = ":";   // What char marks that value is afterwards.
    char comment_indicator = '#';        // What char marks a comment.
    int  ascii_empty_line_indicator = 0; // What value marks that a line is empty (ASCII NULL).

    std::string file_line;
    while(std::getline(input_file, file_line)) {
        if(file_line[0] == comment_indicator || (int)file_line[0] == ascii_empty_line_indicator)
            continue;

        auto index = iso::find(file_line, read_value_from);
        std::string property_name  = iso::readBefore(file_line, read_value_from); 
        std::string property_value = iso::read(file_line, index + 1, file_line.length() - 1);

        if(property_name == "WINDOW_SIZE") {
            auto str_width  = iso::read(file_line, iso::find(file_line, read_value_from) + 1, iso::find(file_line, "x"));
            auto str_height = iso::read(file_line, iso::find(file_line, "x") + 1, iso::find(file_line, line_delimiter));
            int width  = std::stoi(str_width);
            int height = std::stoi(str_height);
            this->m_settings.window_size = sf::Vector2f(width, height);
            std::cout << "[Entropy Engine][Engine Settings]: Window size:\t\t" << width << "x" << height << "\n";
            continue;
        }
    
        if(property_name == "FULLSCREEN") {
            this->m_settings.window_fullscreen = std::stoi(property_value);
            std::cout << "[Entropy Engine][Engine Settings]: Fullscreen:\t\t" << iso::asBool(this->m_settings.window_fullscreen) << "\n";
            continue;
        }

        if(property_name == "VSYNC") {
            this->m_settings.window_vsync = std::stoi(property_value);
            std::cout << "[Entropy Engine][Engine Settings]: Vsync enabled:\t" << iso::asBool(this->m_settings.window_vsync) << "\n";
            continue;
        }

        if(property_name == "REFRESH_RATE") {
            this->m_settings.window_refresh_rate = std::stoi(property_value);
            std::cout << "[Entropy Engine][Engine Settings]: Refresh rate:\t" << this->m_settings.window_refresh_rate << "\n";
            continue;
        }

        if(property_name == "DEBUG_MODE") {
            this->m_settings.application_debug_mode = std::stoi(property_value);
            std::cout << "[Entropy Engine][Engine Settings]: Debug mode:\t\t" << iso::asBool(this->m_settings.application_debug_mode) << "\n";
            continue;
        }
    }

    // Close the file.

    input_file.close();
}

const Settings& applicationSettings::userSettings() {
    return this->m_settings;    
}