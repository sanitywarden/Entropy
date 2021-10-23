#include "entropy.hpp"
#include "gamestate.hpp"

#include <iostream>

using namespace entropy;

Entropy::Entropy() {
    if(this->settings.getUserSettings().window_fullscreen) 
        this->window.createFullscreenWindow();
    
    else 
        this->window.createWindow(this->settings.getUserSettings().window_size);

    this->window.setTitle("Entropy");
    this->window.setVsync(this->settings.getUserSettings().window_vsync);

    std::cout << "[Entropy Engine]: Configuration finished.\n";
    std::cout << "[Entropy Engine]: Greetings from Entropy Game Engine.\n";
}

Entropy::~Entropy() {
    
}

void Entropy::loop() {    
    this->m_clock          = sf::Clock();
    this->m_last_update    = sf::Time::Zero;
    this->m_time_per_frame = sf::Time(sf::seconds(1.f / (float)this->settings.getUserSettings().window_refresh_rate));

    while(this->window.open()) {
        sf::Time elapsed_time = this->m_clock.restart();
        this->m_last_update += elapsed_time;

        while(this->m_last_update > this->m_time_per_frame) {
            this->m_last_update -= this->m_time_per_frame;
            
            auto gamestate = this->gamestate.getGamestate();

            if(gamestate != nullptr) {
                gamestate->update(this->m_time_per_frame.asSeconds());
                gamestate->render(this->m_time_per_frame.asSeconds());
            }
        } 
    }
}

void Entropy::quitApplication(int code) {
    std::cout << "[Entropy Engine]: Application quit with code " << code << ".\n";

    switch(code) {
        default:
            std::cout << "[Entropy Engine]: Unkown exit code.\n";
            break;

        case 0:
            std::cout << "[Entropy Engine]: Application exited successfully.\n";
            break;
        
        case 1:
            std::cout << "[Entropy Engine]: Application exited, error occurred.\n";
            break;
    }

    exit(code);
}