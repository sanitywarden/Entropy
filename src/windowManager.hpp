#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

#include "applicationSettings.hpp"
#include "gameObject.hpp"

namespace iso {
class WindowManager {
    private:
        std::shared_ptr <sf::RenderWindow> window;
    
    private:
        void createFullscreenWindow();
    
    public:
        WindowManager();
        ~WindowManager();
        
        void createWindow(const ApplicationSettings& settings, const std::string& title);

        sf::RenderWindow* getWindow();
        core::Vector2i getWindowSize() const;
        unsigned int getWindowWidth()  const;
        unsigned int getWindowHeight() const;
        
        bool isOpen() const;
        bool isFocused() const;
        bool inScreenSpace(const GameObject& object) const;

        void setTitle(const std::string& title);
        void setVsync(bool on = false);
        void setKeyHold(bool on = false);
        void setMaxFramerate(unsigned int framerate);
        void takeScreenshot() const;
        void setView(const sf::View& view);
        const sf::View& getView() const;

        void clear(const core::Colour& clear_colour);
        void display();
        void draw(const sf::Drawable& drawable, sf::RenderStates states = sf::RenderStates());
        void close();
};
}