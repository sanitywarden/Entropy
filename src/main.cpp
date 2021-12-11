#include "entropy/entropy.hpp"
#include "worldmap.hpp"

int main() {
    static entropy::Entropy game_engine;

    iso::Worldmap worldmap = iso::Worldmap(&game_engine);
    game_engine.gamestate.addGamestate("worldmap", worldmap);
    game_engine.gamestate.setGamestate("worldmap");

    try {
        game_engine.loop();
    } catch(const std::exception& exception) {
        std::cout << exception.what() << "\n";
    }

    entropy::Entropy::quitApplication(0);
}

