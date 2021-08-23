#ifndef _GUI_WIDGET_HPP_
#define _GUI_WIDGET_HPP_

#include "SFML/Graphics.hpp"

#include <functional>

namespace gui {    
    enum class WidgetState {
        STATE_REST,    // Nothing is happening with the widget.
        STATE_PRESSED, // Widget was clicked on.
        STATE_HOVERED, // Mouse cursor is hovered over the widget.
        STATE_MARKED   // Widget is marked or highligted in some way.
    };
    
    // Base class for GUI elements.
    // Notice - this class does not provide the shape of the widget. You have to decide which SFML shape you want to use when inheriting from this class. 
    class Widget : public sf::Drawable {
        private:
            std::string widget_id; // ID for easier use of GUI components.
        
        public:
            sf::Vector2f widget_position; 
            sf::Vector2f widget_size;     
            sf::Vector2f widget_scale;    

        public:
            Widget();
            ~Widget();

            void setWidgetID(const std::string&);
            const std::string& getWidgetID();

            virtual bool containsPoint(const sf::Vector2f&) { return; }
            
            virtual void onMouseButtonPress(std::function<void()>) { return; }
            
            virtual void onMouseButtonRelease(std::function<void()>) { return; } 
    };
}

#endif