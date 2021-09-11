#ifndef _GUI_WIDGET_HPP_
#define _GUI_WIDGET_HPP_

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <functional>

namespace gui {    
    enum class WidgetState {
        STATE_REST,    // Nothing is happening with the widget.
        STATE_PRESSED, // Widget was clicked on.
        STATE_HOVERED, // Mouse cursor is hovered over the widget.
        STATE_MARKED   // Widget is marked or highligted in some way.
    };
    
    // Base class for GUI elements.
    // Notice - this class does not provide the shape of the widget. 
    class AbstractWidget : public sf::Drawable {
        private:
            std::string m_widget_id;
        
        public:
            sf::Vector2f widget_position; 
            sf::Vector2f widget_size;     
            sf::Vector2f widget_scale;    

        public:
            AbstractWidget();
            ~AbstractWidget();

            void setWidgetID(std::string&);
            std::string& getWidgetID();

            virtual bool containsPoint(const sf::Vector2f&)          { return; }
            virtual void onMouseButtonPress(std::function<void()>)   { return; }
            virtual void onMouseButtonRelease(std::function<void()>) { return; }
    };
}

#endif