#ifndef _ENTROPY_ENGINE_SETTINGS_HPP_
#define _ENTROPY_ENGINE_SETTINGS_HPP_

#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>

namespace iso {
    // This is a class that represents the user-customisable application settings.
    // It does not have any public or private functions.
    // Default parameters are set in the class constructor.
    class Settings {
        public:
            Settings();
            ~Settings();

            sf::Vector2f window_size;
            bool         window_fullscreen;
            bool         window_vsync;
            unsigned int window_refresh_rate;
            bool         application_debug_mode;
    };
    
    // This is a class that is responsible for loading and saving user settings whenever the application starts, or ends.
    // The settings are loaded into the application whenever this class is instantinated.
    // The settings are saved to a file whenever the program terminates.
    class applicationSettings {
        private:
            Settings m_settings;
            std::string       m_default_config_path; // Default path where the config file is stored.

            void loadUserSettings();
        public:
            applicationSettings();
            ~applicationSettings();

            const Settings& userSettings() const;
    };  
}

#endif