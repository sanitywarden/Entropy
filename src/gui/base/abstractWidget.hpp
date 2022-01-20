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
            std::string     m_widget_id;
            sf::Vector2f    m_widget_position; 
            sf::Vector2f    m_widget_offset;
            sf::Vector2f    m_widget_size;     
            AbstractWidget* m_parent;
            bool            m_transparent;
        public:
            AbstractWidget();
            ~AbstractWidget();

            void setWidgetID(const std::string id);
            std::string getWidgetID() const;

            void setWidgetPosition(sf::Vector2f position);
            void setWidgetPosition(float x, float y);
            sf::Vector2f getWidgetPosition() const;
            
            void setWidgetOffset(sf::Vector2f offset);
            void setWidgetOffset(float x, float y);
            sf::Vector2f getWidgetOffset() const;

            void setWidgetSize(sf::Vector2f size);
            void setWidgetSize(float x, float y);
            sf::Vector2f getWidgetSize() const;

            void setTransparent(bool transparent);
            bool isTransparent() const;

            void setParent(AbstractWidget* parent);
            AbstractWidget* getParent() const;
            bool attached() const;

            /* By default checks if the mouse is contained inside a widget in the shape of a square / rectangle. */
            virtual bool containsPoint(sf::Vector2f point) const;

        public:
            bool show;
    };
}

#endif