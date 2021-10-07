#ifndef _FPS_COUNTER_HPP_
#define _FPS_COUNTER_HPP_

#include <SFML/System/Clock.hpp>

namespace entropy {
    class FPS {
        private:
            sf::Clock m_clock;
            
            unsigned int m_fps;
            unsigned int m_frame;

        public:
            FPS();
            ~FPS();

            void update();
            unsigned int getFPS();
    };
}

#endif