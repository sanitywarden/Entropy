#ifndef _ENTROPY_HPP_
#define _ENTROPY_HPP_

#include "resourceManager.hpp"
#include "windowManager.hpp"
#include "gamestateManager.hpp"
#include "engineSettings.hpp"

namespace iso {
    /* Entropy is a main class which is supposed to work as a engine.
     * All the modules that you add are supposed to be in Entropy class.
     * To create a application using this engine you need to:
     * - Create a window using Entropy::windowManager. Either fullscreen or custom size.
     * - Create a gamestate with three overloaded functions: update(), render() and handleInput().
     * 
     * Currently it automatically creates a window, with settings provided in config file, if it exists. */
    class Entropy {
        protected:
            sf::Clock m_measurement_clock;
            sf::Time  m_time_since_start;
            int       m_frames_per_second;
            int       m_time_per_frame;
        public:
            Entropy();
            ~Entropy();
            
            resourceManager     resource;
            gamestateManager    gamestate;
            windowManager       window;
            applicationSettings config;

            virtual void loop();
            int getFramesPerSecond();
            int getTimePerFrame();
            void exitApplication(int code);

            
    };
}

#endif
