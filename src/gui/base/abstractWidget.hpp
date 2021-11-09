#ifndef _GUI_ABSTRACT_WIDGET_HPP_
#define _GUI_ABSTRACT_WIDGET_HPP_

#include <SFML/Graphics.hpp>
#include <functional>

namespace gui {    
    enum class WidgetState {
        STATE_REST,    // Nothing is happening with the widget.
        STATE_PRESSED, // Widget was clicked on.
        STATE_HOVERED, // Mouse cursor is hovered over the widget.
        STATE_MARKED   // Widget is marked or highligted in some way.
    };

    // Base class for GUI elements.
    class AbstractWidget : public sf::Drawable {
        private:
            std::string  m_widget_id;
            sf::Vector2f m_widget_position; 
            sf::Vector2f m_widget_size;     
            sf::Vector2f m_widget_scale; 
            sf::Color    m_widget_colour;

            AbstractWidget* m_parent;

            bool m_transparent;

            std::function <void()> m_update_callback;

        public:   
            
        public:
            AbstractWidget();
            ~AbstractWidget();

            void setWidgetID(std::string);
            std::string getWidgetID() const;

            void setWidgetPosition(sf::Vector2f);
            sf::Vector2f widgetPosition() const;
            
            void setWidgetSize(sf::Vector2f);
            sf::Vector2f widgetSize() const;

            void setWidgetScale(sf::Vector2f);
            sf::Vector2f widgetScale() const;

            void setWidgetColour(sf::Color);
            sf::Color widgetColour() const;

            void setTransparent(bool);
            bool isTransparent() const;

            void setParent(AbstractWidget* parent);
            AbstractWidget* parent() const;
            bool attached() const;

            void setCallback(std::function <void()> callback);

            virtual void update();
            virtual bool containsPoint(sf::Vector2f) { return false; }
    };
}

#endif