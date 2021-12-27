#include "entropy.hpp"
#include "gamestate.hpp"

#include <iostream>

using namespace entropy;

Entropy::Entropy() {
    auto settings = this->settings.userSettings();

    this->resource  = resourceManager(settings);
    this->gamestate = gamestateManager(settings);
    this->window    = windowManager(settings);
    
    if(this->settings.userSettings().window_fullscreen) 
        this->window.createFullscreenWindow();
    
    else 
        this->window.createWindow(this->settings.userSettings().window_size);

    this->window.setTitle("Entropy");
    this->window.setVsync(this->settings.userSettings().window_vsync);
    this->window.setMaxFramerate(this->settings.userSettings().window_refresh_rate);

    std::cout << "[Entropy Engine]: Configuration finished.\n";
    std::cout << "[Entropy Engine]: Greetings from Entropy Game Engine.\n";   
}

Entropy::~Entropy() {
    
}

void Entropy::loop() {        
    this->m_measurement_clock = sf::Clock();
    const sf::Time one_second = sf::seconds(1.0f);
    
    // Frames per second.
    uint16_t updates = 0;

    while(this->window.open()) {
        if(this->m_time_since_start.asMilliseconds() < one_second.asMilliseconds()) {
            // Measure the difference in time between the last frame and this frame.
            const float delta_time   = this->m_measurement_clock.getElapsedTime().asMilliseconds() - this->m_time_since_start.asMilliseconds();            
            this->m_time_since_start = this->m_measurement_clock.getElapsedTime();
            updates++;

            Gamestate* gamestate = this->gamestate.getGamestate();
            if(gamestate != nullptr) {
                gamestate->update(delta_time);
                gamestate->render(delta_time);
            }
        }

        else {
            this->m_frames_per_second = updates;
            this->m_time_per_frame    = (float)this->m_time_since_start.asMilliseconds() / (float)this->m_frames_per_second;

            this->m_measurement_clock = sf::Clock();
            this->m_time_since_start  = sf::Time::Zero;
            updates = 0;
        }
    }
}

uint16_t Entropy::getFramesPerSecond() {
    return this->m_frames_per_second;
}

uint32_t Entropy::getTimePerFrame() {
    return this->m_time_per_frame;
}

void Entropy::exitApplication(int code) {
    std::cout << "[Entropy Engine]: Requested application exit with code " << code << ".\n";
    exit(code);
}