#ifndef _ENTROPY_WINDOW_MANAGER_HPP_
#define _ENTROPY_WINDOW_MANAGER_HPP_

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

#include "engineSettings.hpp"

namespace entropy {
    // Window Manager is a class responsible for creating and managing a SFML window.
    class windowManager {
        private:
            Settings m_settings;

            std::shared_ptr <sf::RenderWindow> m_window;
        public:
            windowManager();
            windowManager(const Settings& settings);
            ~windowManager();
            
            void createFullscreenWindow();
            void createWindow(sf::Vector2f window_size);

            sf::RenderWindow* getWindow();
            sf::Vector2f      windowSize();
            unsigned int      windowWidth();
            unsigned int      windowHeight();
            bool open();
            bool focused();

            void setTitle(std::string title);
            void setVsync(bool on = false);
            void setKeyHold(bool on = false);
            void setMaxFramerate(unsigned int framerate);

            void clear(const sf::Color& clear_colour);
            void display();
            void draw(const sf::Drawable& drawable, sf::RenderStates states = sf::RenderStates());
            void close();
    };
}

#endif