#pragma once

#include "resourceManager.hpp"
#include "windowManager.hpp"
#include "gamestateManager.hpp"
#include "applicationSettings.hpp"

namespace iso {
void exitApplication(int code);

class Entropy {
    protected:
        sf::Clock measurement_clock;
        sf::Time  time_since_start;
        int       frames_per_second;
        int       time_per_frame;
    public:
        Entropy();
        ~Entropy();
        
        ResourceManager  resource;
        GamestateManager gamestate;
        WindowManager    window;

        virtual void setup();
        virtual void loop();

        int getFramesPerSecond();
        int getTimePerFrame();
};
}
