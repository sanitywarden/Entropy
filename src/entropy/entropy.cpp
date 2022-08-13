#include "entropy.hpp"
#include "gamestate.hpp"

#include <iostream>

using namespace iso;

Entropy::Entropy() 
{}

Entropy::~Entropy() 
{}

void Entropy::setup() 
{}

void Entropy::loop() {        
    this->measurement_clock = sf::Clock();
    const auto one_second = sf::seconds(1.0f);
    
    // Frames per second.
    int updates = 0;

    while(this->window.isOpen()) {
        if(this->time_since_start.asMilliseconds() < one_second.asMilliseconds()) {
            // Measure the difference in time between the last frame and this frame.
            float delta_time = this->measurement_clock.getElapsedTime().asMilliseconds() - this->time_since_start.asMilliseconds();            
            this->time_since_start = this->measurement_clock.getElapsedTime();
            updates++;

            /* Delta time is saved as milliseconds, convert that to seconds.
             * 1ms -> 0.001s. */
            float delta_time_s = delta_time / 1000;

            auto* gamestate = this->gamestate.getGamestate();
            if(gamestate == nullptr) {
                std::cout << "[Entropy Engine]: Current gamestate is a nullptr.\n";
                exitApplication(1);
            }

            if(gamestate->engine == nullptr) {
                std::cout << "[Entropy Engine] Engine is a nullptr.\n";
                exitApplication(1);
            }

            gamestate->update(delta_time_s);
            gamestate->render(delta_time_s);
        }

        else {
            this->frames_per_second = updates;
            this->time_per_frame    = (float)this->time_since_start.asMilliseconds() / (float)this->frames_per_second;

            this->measurement_clock.restart();
            this->time_since_start = sf::Time::Zero;
            updates = 0;
        }
    }
}

int Entropy::getFramesPerSecond() {
    return this->frames_per_second;
}

int Entropy::getTimePerFrame() {
    return this->time_per_frame;
}

void iso::exitApplication(int code) {
    std::cout << "[] Closed application with code: " << code << "\n";
    exit(code);
}
