#ifndef _GUI_IMAGE_HPP_
#define _GUI_IMAGE_HPP_

#include "abstractWidget.hpp"
#include "simulationManager.hpp"

#include <SFML/Graphics.hpp>
#include <string>

namespace gui {
    class ImageHolder : public AbstractWidget {
        private:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        private:
            iso::SimulationManager* manager;
            std::string texture_name;
        public:
            ImageHolder();
            ImageHolder(iso::SimulationManager* manager, std::string texture_name);
            ~ImageHolder();
    };
}

#endif