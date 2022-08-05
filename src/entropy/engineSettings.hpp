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

            sf::Vector2i window_size;
            bool         window_fullscreen;
            bool         window_vsync;
            unsigned int window_refresh_rate;
            bool         application_debug_mode;
    };
}

#endif