#ifndef _PANEL_FEATURE_HPP_
#define _PANEL_FEATURE_HPP_

#include <SFML/Graphics/Texture.hpp>

namespace iso {
    class panelFeature {
        private:

        public:
            panelFeature() {
                this->texture = nullptr;
            }
            
            sf::Texture* texture;
    };
}

#endif