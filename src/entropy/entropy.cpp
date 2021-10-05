#include "entropy.hpp"
#include "gamestate.hpp"

#include <iostream>

entropy::Entropy::Entropy() {
    if(this->settings.getUserSettings().window_fullscreen) this->window.createFullscreenWindow();
    else this->window.createWindow(this->settings.getUserSettings().window_size);

    std::cout << "[Entropy Engine]: Configuration finished.\n";
    std::cout << "[Entropy Engine]: Greetings from Entropy Game Engine.\n";
}

entropy::Entropy::~Entropy() {}

void entropy::Entropy::loop() {    
    this->fps = entropy::FPS();

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

void entropy::Entropy::quitApplication(int code, bool close_console) {
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

    if(close_console)
        exit(code);
}