#include "entropy.hpp"
#include "gamestate.hpp"

#include <iostream>

using namespace entropy;

Entropy::Entropy() {
    auto settings = this->config.userSettings();

    this->resource  = resourceManager(settings);
    this->gamestate = gamestateManager(settings);
    this->window    = windowManager(settings);
    
    if(this->config.userSettings().window_fullscreen) 
        this->window.createFullscreenWindow();
    
    else 
        this->window.createWindow(this->config.userSettings().window_size);

    this->window.setTitle("Entropy Application");

    this->window.setVsync(this->config.userSettings().window_vsync);
    this->window.setMaxFramerate(this->config.userSettings().window_refresh_rate);

    std::cout << "[Entropy Engine]: Configuration finished.\n";
    std::cout << "[Entropy Engine]: Greetings from Entropy Game Engine.\n";   
}

Entropy::~Entropy() {
    
}

void Entropy::loop() {        
    this->m_measurement_clock = sf::Clock();
    const sf::Time one_second = sf::seconds(1.0f);
    
    // Frames per second.
    int updates = 0;

    while(this->window.open()) {
        if(this->m_time_since_start.asMilliseconds() < one_second.asMilliseconds()) {
            // Measure the difference in time between the last frame and this frame.
            const float delta_time   = this->m_measurement_clock.getElapsedTime().asMilliseconds() - this->m_time_since_start.asMilliseconds();            
            this->m_time_since_start = this->m_measurement_clock.getElapsedTime();
            updates++;

            /* Delta time is saved as milliseconds, convert that to seconds.
             * 1ms -> 0.001s. */
            const float delta_time_s = delta_time / 1000;

            Gamestate* gamestate = this->gamestate.getGamestate();
            if(gamestate == nullptr) {
                std::cout << "[Entropy Engine]: Current gamestate is a nullptr.\n";
                return;
            }

            if(gamestate->engine == nullptr) {
                std::cout << "[Entropy Engine] Current gamestate created without engine.\n";
                return;
            }

            gamestate->update(delta_time_s);
            gamestate->render(delta_time_s);
        }

        else {
            this->m_frames_per_second = updates;
            this->m_time_per_frame    = (float)this->m_time_since_start.asMilliseconds() / (float)this->m_frames_per_second;

            this->m_measurement_clock.restart();
            this->m_time_since_start = sf::Time::Zero;
            updates = 0;
        }
    }
}

int Entropy::getFramesPerSecond() {
    return this->m_frames_per_second;
}

int Entropy::getTimePerFrame() {
    return this->m_time_per_frame;
}

void Entropy::exitApplication(int code) {
    std::cout << "[Entropy Engine]: Requested application exit with code " << code << ".\n";
    exit(code);
}
