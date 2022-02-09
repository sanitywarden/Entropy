#include "simulationManager.hpp"
#include "entropy/gamestate.hpp"
#include "worldmap.hpp"
#include "regionmap.hpp"

#include <iostream>

using namespace iso;
using namespace entropy;

static int LOOPS = 0; // Number of loops that the program has run.
static int SUM   = 0; // Sum of the FPS. Used for averaging.

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

    if(!this->performChecks()) {
        std::cout << "[Initialisation]: Application checks failed.\n";
        this->exitApplication(1);
    }

    static Worldmap worldmap = Worldmap(this);
    this->gamestate.addGamestate("worldmap", worldmap);

    this->world = WorldGenerator(&this->resource, this->settings);
    
    this->prepare();

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
            
            if(this->time.time_passed < INT_MAX) {
                this->time.time_passed++;

                SUM += updates;
                LOOPS++;

                this->average_fps = SUM / LOOPS;
            }

            this->time.calculateInGameDate();

            updates = 0;
        }
    }
}

void SimulationManager::internalLoop(float delta_time) {
    Gamestate* gamestate = this->gamestate.getGamestate();
    if(gamestate == nullptr) {
        std::cout << "[Simulation Manager]: Gamestate is a nullptr.\n";
        this->exitApplication(1);
    } 

    // Does not matter what gamestate it is, player always decides before the AI.
    gamestate->update(delta_time);
    gamestate->render(delta_time);

    const std::string& gamestate_name = gamestate->state_id;
    for(const auto& player : this->players) {
        // Player updates are tied to gamestate updates.
        if(player.isHuman())
            continue;

        /* Handle AI updates here. */
        
    }
}

void SimulationManager::spawnPlayers() {    
    std::srand(std::time(0));

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
                
                Region& region = this->world.world_map[index];
                // this->world.generateRegion(index, region);

                Player& player = this->players[current_player]; 
                region.owner = &player;

                spot_found = true;
                break;
            }
        }
        
        this->players.at(current_player).team_colour = TEAM_COLOURS[current_player];
        this->players.at(current_player).addOwnedRegion(claimed_spots.back());
        current_player++;
    }

    for(auto& player : this->players) {
        this->world.world_map[player.getCapital()].object_colour = player.team_colour;
    }

    this->players[0].setHuman(true);
    this->players[0].setCountryName("Sanity Wardens");
    std::cout << "[Simulation Manager]: Starting locations found.\n";    
}

Player& SimulationManager::getHumanPlayer() {
    return this->players[0];
}

void SimulationManager::prepare() {
    this->world.generateWorld();
    this->spawnPlayers();
}

struct AStarNode {
    int x;
    int y;
    int index;
    int cost_heuristic;
    int cost_sincestart;
    bool visited;

    AStarNode* parent;
    std::vector <AStarNode> neighbours;

    AStarNode() : index(0), cost_heuristic(9999), cost_sincestart(9999), visited(false), parent(nullptr) {};
};

/* A* algorithm implementation. 
 * Returns a vector with indexes of tiles forming a path. */
std::vector <int> SimulationManager::astar(int start_index, int end_index) {
    const auto distance = [](AStarNode& node1, AStarNode& node2) -> int {
        return std::abs(node1.x - node2.x) + std::abs(node1.y - node2.y);
    };

    const auto heuristic = [distance](AStarNode& node1, AStarNode& node2) -> int {
        return distance(node1, node2);
    };

    // One million.
    const int ASTAR_LIMIT = 1000000;

    std::queue  <AStarNode> list_to_check;
    std::vector <AStarNode> discovered_tiles(this->settings.region_size * this->settings.region_size);
    std::vector <int>  solution;

    AStarNode node;
    for(int y = 0; y < this->settings.region_size; y++) {
        for(int x = 0; x < this->settings.region_size; x++) {
            const int index = y * this->settings.region_size + x;
            node.x     = x;
            node.y     = y;
            node.index = index;
            discovered_tiles[index] = node;
        }
    }


    solution.push_back(start_index);

    AStarNode& start = discovered_tiles[start_index];
    AStarNode& end   = discovered_tiles[end_index];    

    start.cost_sincestart = 0;
    start.cost_heuristic  = heuristic(start, end);

    list_to_check.push(start);
    int counter = 0;

    AStarNode* current = nullptr;
    while(!list_to_check.empty()) {
        if(list_to_check.empty())
            break;
    
        if(counter == ASTAR_LIMIT) {
            std::cout << "[Simulation Manager][A*]: Astar limit reached.\n";
            break;
        }

        current = &list_to_check.front();
        current->visited = true;

        list_to_check.pop();

        if(current->index == end.index)
            break;

        // Find neighbours.
        if(current->index - 1 >= 0) {
            auto& neighbour = discovered_tiles[current->index - 1];
            if(current->y == neighbour.y)
                current->neighbours.push_back(neighbour);
        }

        if(current->index + 1 < this->world.getRegionSize()) {
            auto& neighbour = discovered_tiles[current->index + 1];
            if(current->y == neighbour.y)
                current->neighbours.push_back(neighbour);
        }

        if(current->index - this->settings.region_size >= 0) {
            auto& neighbour = discovered_tiles[current->index - this->settings.region_size];
            current->neighbours.push_back(neighbour);
        }

        if(current->index + this->settings.region_size < this->world.getRegionSize()) {
            auto& neighbour = discovered_tiles[current->index + this->settings.region_size];
            current->neighbours.push_back(neighbour);
        }
        
        int smallest_cost = 9999;
        int index         = -1;

        for(int i = 0; i < current->neighbours.size(); i++) {
            auto& neighbour = current->neighbours[i];

            neighbour.cost_sincestart = current->cost_sincestart + 1;
            neighbour.cost_heuristic  = heuristic(neighbour, end);

            // Try to find the best possible tile to continue pathing through.
            if(!neighbour.visited && neighbour.cost_heuristic <= smallest_cost) {
                smallest_cost     = neighbour.cost_heuristic;
                index             = neighbour.index;
                neighbour.visited = true;
            }

            if(i == current->neighbours.size() - 1 && index != -1) {
                list_to_check.push(discovered_tiles[index]);
                solution.push_back(index);
            }
        }

        counter++;
    }

    return solution;
}

int SimulationManager::getAverageFramesPerSecond() {
    return this->average_fps;
}

bool SimulationManager::performChecks() {
    #ifdef ENTROPYAPP_STACKSIZE
    std::cout << "[Initialisation]: Max stack size allocation:   " << ENTROPYAPP_STACKSIZE / 1000000 << " MB\n";

    const int tile_byte_size = sizeof(Tile);
    std::cout << "[Initialisation]: Size of one tile instance:   " << tile_byte_size << " B\n";
    
    const int region_byte_size = sizeof(Region);
    std::cout << "[Initialisation]: Size of one region instance: " << region_byte_size << " B\n";

    const int regionmap_byte_size = settings.region_size * settings.region_size * tile_byte_size;
    std::cout << "[Initialisation]: MB size of regionmap:        " << (float)regionmap_byte_size / 1000000 << " MB\n";
    
    const int worldmap_byte_size = settings.world_size * settings.world_size * region_byte_size;
    std::cout << "[Initialisation]: MB size of worldmap:         " << (float)worldmap_byte_size / 1000000 << " MB\n";

    if(regionmap_byte_size > ENTROPYAPP_STACKSIZE) {
        std::cout << "[Initialisation]: Memory size of region map is greater than the stack allocation limit: " << regionmap_byte_size / 1000000 << " / " << ENTROPYAPP_STACKSIZE;
        return false;
    }

    if(worldmap_byte_size > ENTROPYAPP_STACKSIZE) {
        std::cout << "[Initialisation]: Memory size of world map is greater than the stack allocation limit:  " << worldmap_byte_size / 1000000 << " / " << ENTROPYAPP_STACKSIZE;
        return false;
    }
    #endif

    return true;
}

