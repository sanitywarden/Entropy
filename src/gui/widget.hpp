#ifndef _GUI_WIDGET_HPP_
#define _GUI_WIDGET_HPP_

#include "abstractWidget.hpp"
#include "button.hpp"

#include <SFML/Graphics.hpp>
#include <functional>
#include <map>

namespace gui {
    // Widget is a interface component serving the role of a frame for other components.
    // You can attach other components to it.
    class Widget : public AbstractWidget {
        private:
            std::map <std::string, gui::AbstractWidget*> m_attached_components;

        private:
            void draw(sf::RenderTarget&, sf::RenderStates) const override;

        public:
            Widget();
            ~Widget();

            // When attaching a component it's position becomes relative to the position of the main widget.
            void attachComponent(gui::AbstractWidget* component, std::string component_name);
            const gui::AbstractWidget& getComponentByName(std::string component_name) const;

            void update()                                    override;
            void onMouseButtonPress(std::function<void()>)   override;
            void onMouseButtonRelease(std::function<void()>) override;
            bool containsPoint(sf::Vector2f)                 override;
    };
}

#endif