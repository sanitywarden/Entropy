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
    entropy::Gamestate* gamestate = &this->gamestate.getGamestate();
    
    this->fps = entropy::FPS();

    while(true) {
        fps.update();

        if(gamestate != nullptr) {
            gamestate->handleInput();
            gamestate->update();
            gamestate->render();
        }
    }
}

static void entropy::Entropy::quitApplication(const int& code) {
    std::cout << "[Entropy Engine]: Application quit with code " << code << ".\n";

    switch(code) {
        default:
            std::cout << "[Entropy Engine]: Unkown exit code.\n";
            break;

        case 0:
            std::cout << "[Entropy Engine]: Application exited successfully.\n";
            break;
        
        case 1:
            std::cout << "[Entropy Engine]: Failed to load a resource file.\n";
            break;
    }

    exit(code);
}