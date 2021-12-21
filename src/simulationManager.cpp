#include "simulationManager.hpp"
#include "entropy/gamestate.hpp"
#include "worldmap.hpp"
#include "regionmap.hpp"

#include <iostream>

using namespace iso;
using namespace entropy;

SimulationManager::SimulationManager() {
    this->world_settings.size.x                 = 100;
    this->world_settings.size.y                 = 100;
    this->world_settings.margin.x               = this->world_settings.size.x / 10;
    this->world_settings.margin.y               = this->world_settings.size.y / 10;
    this->world_settings.panel_size.x           = 64;
    this->world_settings.panel_size.y           = 32;
    this->world_settings.noise_octaves          = 16;
    this->world_settings.noise_persistence      = 4;
    this->world_settings.noise_bias             = 4;
    this->world_settings.moisture_octaves       = 8;
    this->world_settings.moisture_persistence   = 8;
    this->world_settings.moisture_bias          = 4;
    this->world_settings.minimum_terrain_height = 0.3f;
    this->world_settings.river_quantity         = (this->world_settings.size.x * this->world_settings.size.y) / ((this->world_settings.size.x * this->world_settings.size.y) / ((this->world_settings.margin.x + this->world_settings.margin.y) / 2));
    this->world_settings.pole_size              = this->world_settings.size.y / 10;
    this->world_settings.multiplier_noise       = 1.25f; // Magic number to make the world look as good as possible.
    this->world_settings.multiplier_gradient    = 2.00f; // Magic number to make the world look as good as possible.
    this->world_settings.multiplier_moisture    = 0.90f; // Magic number to make the world look as good as possible.

    this->region_settings.size.x        = 100;
    this->region_settings.size.y        = 100;
    this->region_settings.tile_offset.x = 100;
    this->region_settings.tile_offset.y = 100;
    this->region_settings.tile_size.x   = 64;
    this->region_settings.tile_size.y   = 32;

    static Worldmap worldmap = Worldmap(this);
    this->gamestate.addGamestate("worldmap", worldmap);
    this->gamestate.setGamestate("worldmap");

    this->world = worldGenerator(&this->resource, this->world_settings, this->region_settings);
    this->world.generateWorld();

    this->spawnPlayers();
    this->players[0].setHuman(true);
}

SimulationManager::~SimulationManager() {

}

void SimulationManager::loop() {
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

            this->internalLoop(delta_time); 
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

void SimulationManager::internalLoop(float delta_time) {
    // Write this as if it was for singular use.
    // It is placed inside a loop that runs the application.

    entropy::Gamestate* gamestate = this->gamestate.getGamestate();
    if(gamestate == nullptr) {
        std::cout << "[Simulation Manager]: Gamestate is a nullptr.\n";
        this->exitApplication(1);
    } 

    // Does not matter what gamestate it is, player always decides before the AI.
    gamestate->update(delta_time);
    gamestate->render(delta_time);

    const std::string gamestate_name = gamestate->getStateID();
    if(gamestate_name == "Worldmap") {

    }

    else if(gamestate_name == "Regionmap") {
        for(auto& player : this->players) {
            // Player updates are tied to gamestate updates.
            if(player.isHuman())
                continue;

            // AI updates are handled here.
            else {
                
            }
        }
    }
}

void SimulationManager::spawnPlayers() {    
    std::srand(time(0));

    const int number_of_players = team_colours.size();
    this->players.resize(number_of_players);
    std::cout << "[Simulation Manager]: Manager will prepare " << number_of_players << " starting locations.\n";
    std::vector <int> claimed_spots(number_of_players); // Storage for already claimed spots, these will be the capitals of player countries.
    int current_player = 0;
    while(current_player != number_of_players) {
        bool spot_found = false;
        while(!spot_found) {
            int index = std::rand() % this->world.getWorldSize();

            if(!this->world.is_arctic(index) && !this->world.is_ocean(index) && !this->world.is_sea(index)) {
                claimed_spots.push_back(index);
                spot_found = true;
                break;
            }
        }
    
        this->players.at(current_player).player_colour = team_colours[current_player];
        this->players.at(current_player).addOwnedRegion(claimed_spots.back());
        current_player++;
    }

    for(auto player : this->players) {
        this->world.world_map[player.resources.owned_regions[0]].colour = player.player_colour;
    }

    std::cout << "[Simulation Manager]: Starting locations found.\n";
}

// void SimulationManager::regionmapUpdateTile(Player& player) {    
//     Regionmap* gamestate = static_cast<Regionmap*>(this->gamestate.getGamestate());
//     gamestate->controls.addKeyMappingCheck("key_cut_tree", sf::Keyboard::Key::D);    
//     if(gamestate->controls.isKeyPressed("key_cut_tree")) {
//         if(gamestate->current_index >= 0 && gamestate->current_index < this->world.getWorldSize()) {
//             player.resources.wood += TREE_WOOD_HARVEST;

//             Region* region = gamestate->getCurrentRegion();
//             region->trees.erase(gamestate->current_index);
//         }
//     }
// }

// void SimulationManager::regionmapPlaceBuilding(Player& player) {
//     Regionmap* gamestate = static_cast<Regionmap*>(this->gamestate.getGamestate());
//     gamestate->controls.addKeyMapping("key_place_building", sf::Keyboard::Key::B);
//     if(gamestate->controls.isKeyPressed("key_place_building")) {
//         if(gamestate->current_index >= 0 && gamestate->current_index < this->world.getWorldSize()) {
//             if(player.resources.wood >= 10) {

//                 Region* region = gamestate->getCurrentRegion();
//                 // region->trees.erase(gamestate->current_index);

//                 player.resources.wood -= 10;
//             } 
//         }
//     }
// }