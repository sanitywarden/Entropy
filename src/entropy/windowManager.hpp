#ifndef _ENTROPY_WINDOW_MANAGER_HPP_
#define _ENTROPY_WINDOW_MANAGER_HPP_

#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

namespace entropy {
    // Window Manager is a class responsible for creating and managing a SFML window.
    class windowManager {
        private:
            std::shared_ptr <sf::RenderWindow> m_window;

        public:
            windowManager();
            ~windowManager();
            
            void createFullscreenWindow();
            void createWindow(const sf::Vector2f& window_size);

            sf::RenderWindow* getWindow();
            sf::Vector2f getWindowSize();

            void clear(const sf::Color&);
            void display();
            //void draw(sf::RenderTarget&, sf::RenderStates);
    };
}

#endif