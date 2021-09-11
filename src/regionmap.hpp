#ifndef _REGIONMAP_HPP_
#define _REGIONMAP_HPP_

#include "entropy/gamestate.hpp"
#include "entropy/entropy.hpp"
#include "region.hpp"
#include "worldGenerator.hpp"

#include <SFML/Graphics.hpp>

namespace iso {
    // Gamestate class responsible for management of a single region.
    class Regionmap : public entropy::Gamestate {
        private:
            Region m_region;

        private:
            void handleInput()         override;
            void initialise()          override;
            void loadResources()       override;
            void updateMousePosition() override;
            void moveCamera()          override;
            void zoomCamera()          override;
            void updateCamera()        override;

        public: 
            /* Regionmap's constructor is triggered every time the user visits a region. 
               This is because you somehow need to update the information about the state of the world, and the region the user visits. */
            Regionmap();
            Regionmap(entropy::Entropy* engine, iso::worldGenerator* world, int region_index);
            ~Regionmap();

            void update() override;
            void render() override;
    }; 
}

#endif