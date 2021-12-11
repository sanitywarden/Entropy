#include "entropy/entropy.hpp"
#include "worldmap.hpp"

#include <conio.h> // Included for getch().

int main() {
    static entropy::Entropy game_engine;

    iso::Worldmap worldmap = iso::Worldmap(&game_engine);
    game_engine.gamestate.addGamestate("worldmap", worldmap);
    game_engine.gamestate.setGamestate("worldmap");

    try {
        game_engine.loop();
    } catch(const std::exception& exception) {
        game_engine.exitApplication(1);
        std::cout << exception.what() << "\n";
    }
}

