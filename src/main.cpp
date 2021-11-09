#include "entropy/entropy.hpp"

#include "worldmap.hpp"
// #include "menu.hpp"

int main() {
    static entropy::Entropy game_engine;

    iso::Worldmap worldmap = iso::Worldmap(&game_engine);
    game_engine.gamestate.addGamestate("worldmap", worldmap);
    game_engine.gamestate.setGamestate("worldmap");

    // iso::Menu menu = iso::Menu(&game_engine);
    // game_engine.gamestate.addGamestate("menu", menu);
    // game_engine.gamestate.setGamestate("menu");

    try {
        game_engine.loop();
    }  catch(const std::exception& exception) {
        std::cout << exception.what() << "\n";
    }

    entropy::Entropy::quitApplication(0);
}

