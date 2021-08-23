#ifndef _ENTROPY_ENGINE_SETTINGS_HPP_
#define _ENTROPY_ENGINE_SETTINGS_HPP_

#include <iostream>
#include <string>
#include <fstream>

#include <SFML/Graphics.hpp>

namespace entropy {
    // This is a class that represents the user-customisable application settings.
    // It does not have any public or private functions.
    // Default parameters are set in the class constructor.
    class Settings {
        public:
            Settings();
            ~Settings();

            sf::Vector2f window_size;
            bool         window_fullscreen;
    };
    
    // This is a class that is responsible for loading and saving user settings whenever the application starts, or ends.
    // The settings are loaded into the application whenever this class is instantinated.
    // The settings are saved to a file whenever the program terminates.
    class applicationSettings {
        private:
            entropy::Settings m_settings;
            std::string   m_default_config_path; // Default path where the config file is stored.

            const std::string extractPartString(const int& from, const int& to, const std::string& str);

            void loadUserSettings();
            void saveUserSettings();

        public:
            applicationSettings();
            ~applicationSettings();

            const entropy::Settings& getUserSettings();
    };  
}

#endif