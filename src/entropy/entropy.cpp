#include "entropy.hpp"
#include "gamestate.hpp"

#include <iostream>

using namespace entropy;

Entropy::Entropy() {
    if(this->settings.getUserSettings().window_fullscreen) this->window.createFullscreenWindow();
    else this->window.createWindow(this->settings.getUserSettings().window_size);

    std::cout << "[Entropy Engine]: Configuration finished.\n";
    std::cout << "[Entropy Engine]: Greetings from Entropy Game Engine.\n";
}

Entropy::~Entropy() {}

void Entropy::loop() {    
    this->fps = FPS();

    while(this->window.open()) {
        auto gamestate = this->gamestate.getGamestate();
    
        if(gamestate != nullptr) {
            gamestate->handleInput();
            gamestate->update();
            gamestate->render();
        }

        fps.update();
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