#include "entropy/entropy.hpp"

#include "worldmap.hpp"

int main() {
    static entropy::Entropy game_engine;

    iso::Worldmap worldmap = iso::Worldmap(&game_engine);
    game_engine.gamestate.addGamestate("worldmap", worldmap);

    game_engine.gamestate.setGamestate("worldmap");

    game_engine.loop();

    entropy::Entropy::quitApplication(0);
}

