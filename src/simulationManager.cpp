#include "simulationManager.hpp"
#include "entropy/gamestate.hpp"
#include "worldmap.hpp"
#include "regionmap.hpp"

#include <iostream>

using namespace iso;
using namespace entropy;

SimulationManager::SimulationManager() {
    GenerationSettings settings;
    settings.world_size                 = 100;
    settings.world_margin_island        = settings.world_size / 10;
    settings.world_margin_poles         = settings.world_size / 10;
    settings.world_river_quantity       = settings.world_size / 10;
    settings.world_river_scan_size      = settings.world_size / 20;
    settings.world_noise_terrain        = 0.3f;
    settings.world_noise_forest         = 0.7f;
    settings.world_panel_size.x         = 64;
    settings.world_panel_size.y         = 32;
    settings.world_noise_octaves        = 16;
    settings.world_noise_persistence    = 4;
    settings.world_noise_bias           = 4;
    settings.world_noise_multiplier     = 1.25f;
    settings.world_moisture_octaves     = 8;
    settings.world_moisture_persistence = 8;
    settings.world_moisture_bias        = 4;
    settings.world_moisture_multiplier  = 0.90f;
    settings.world_gradient_octaves     = 16;
    settings.world_gradient_persistence = 4;
    settings.world_gradient_bias        = 4;
    settings.world_gradient_multiplier  = 2.00f;
    settings.region_size                = 100;
    settings.region_tile_size.x         = 64;           
    settings.region_tile_size.y         = 32;           
    settings.region_tile_offset.x       = 0; 
    settings.region_tile_offset.y       = 0; 

    this->settings = settings;
    // Settings for world generation set.

    this->window.setTitle("Entropy by Vivit");

    static Worldmap worldmap = Worldmap(this);
    this->gamestate.addGamestate("worldmap", worldmap);

    this->world = WorldGenerator(&this->resource, this->settings);
    this->world.generateWorld();
    
    this->spawnPlayers();

    this->gamestate.setGamestate("worldmap");
}

SimulationManager::~SimulationManager() {

}

void SimulationManager::loop() {
    this->m_measurement_clock = sf::Clock();
    const sf::Time one_second = sf::seconds(1.0f);
    
    // Frames per second.
    int updates = 0;

    while(this->window.open()) {
        if(this->m_time_since_start.asMilliseconds() < one_second.asMilliseconds()) {
            // Measure the difference in time between the last frame and this frame.
            const float delta_time   = this->m_measurement_clock.getElapsedTime().asMilliseconds() - this->m_time_since_start.asMilliseconds();            
            this->m_time_since_start = this->m_measurement_clock.getElapsedTime();
            updates++;

            /* Delta time is saved as milliseconds, convert that to seconds.
             * 1 ms -> 0.001s. */
            const float delta_time_s = delta_time / 1000;
            this->internalLoop(delta_time_s); 
        }

        else {
            this->m_frames_per_second = updates;
            this->m_time_per_frame    = (float)this->m_time_since_start.asMilliseconds() / (float)this->m_frames_per_second;

            this->m_measurement_clock.restart();
            this->m_time_since_start = sf::Time::Zero;
            updates = 0;
        }
    }
}

void SimulationManager::internalLoop(float delta_time) {
    entropy::Gamestate* gamestate = this->gamestate.getGamestate();
    if(gamestate == nullptr) {
        std::cout << "[Simulation Manager]: Gamestate is a nullptr.\n";
        this->exitApplication(1);
    } 

    // Does not matter what gamestate it is, player always decides before the AI.
    gamestate->update(delta_time);
    gamestate->render(delta_time);

    const std::string gamestate_name = gamestate->state_id;
    for(auto& player : this->players) {
        // Player updates are tied to gamestate updates.
        if(player.isHuman())
            continue;

        // AI updates are handled here.
        else {
            if(gamestate_name == "Worldmap") {

            }

            else if(gamestate_name == "Regionmap") {
            
            }
        }
    }
}

void SimulationManager::spawnPlayers() {    
    std::srand(time(0));

    const int number_of_players = TEAM_COLOURS.size();
    this->players.resize(number_of_players);
    std::cout << "[Simulation Manager]: Manager will prepare " << number_of_players << " starting locations.\n";
    std::vector <int> claimed_spots(number_of_players); // Storage for already claimed spots, these will be the capitals of player countries.
    int current_player = 0;
    while(current_player != number_of_players) {
        bool spot_found = false;
        while(!spot_found) {
            const int index = std::rand() % this->world.getWorldSize();
            if(!this->world.is_arctic(index) && !this->world.is_ocean(index) && !this->world.is_sea(index)) {
                claimed_spots.push_back(index);
                spot_found = true;
                break;
            }
        }
        
        this->players.at(current_player).team_colour = TEAM_COLOURS[current_player];
        this->players.at(current_player).addOwnedRegion(claimed_spots.back());
        current_player++;
    }

    for(auto& player : this->players) {
        this->world.world_map[player.owned_regions[0]].colour = player.team_colour;
    }

    this->players[0].setHuman(true);
    std::cout << "[Simulation Manager]: Starting locations found.\n";    
}

Player& SimulationManager::getHumanPlayer() {
    return this->players[0];
}