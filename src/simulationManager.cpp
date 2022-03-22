#include "simulationManager.hpp"
#include "entropy/gamestate.hpp"
#include "worldmap.hpp"
#include "regionmap.hpp"

#include <iostream>

using namespace iso;
using namespace entropy;

SimulationManager::SimulationManager() {
    GenerationSettings settings;
    settings.world_size                 = 50;
    settings.world_margin_island        = settings.world_size / 10;
    settings.world_margin_poles         = settings.world_size / 10;
    settings.world_river_quantity       = settings.world_size / 10;
    settings.world_river_scan_size      = settings.world_size / 20;
    settings.world_noise_terrain        = 0.20f;
    settings.world_noise_forest         = 0.70f;
    settings.world_panel_size.x         = 128;
    settings.world_panel_size.y         = 128;
    settings.world_noise_octaves        = 16;
    settings.world_noise_persistence    = 10;
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
    this->window.setKeyHold(false);

    static Worldmap worldmap = Worldmap(this);
    this->gamestate.addGamestate("worldmap", worldmap);
    this->world = WorldGenerator(&this->resource, this->settings);
    this->prepare();
    this->gamestate.setGamestate("worldmap");

    this->global_updates.insert({ "update_buildings", std::pair(0, 5) });
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
            
            if(this->time < INT_MAX)
                this->time++;

            Gamestate* gamestate = this->gamestate.getGamestate();
            // Update gamestate-specific scheduler.
            if(gamestate)
                gamestate->updateScheduler();
            
            // Update global scheduler.
            this->updateScheduler();

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

    // Player updates.
    gamestate->update(delta_time);
    gamestate->render(delta_time);

    // AI updates.
}

void SimulationManager::spawnPlayers() {    
    return;
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
            if(!this->world.is_arctic(index) && !this->world.is_ocean(index) && !this->world.is_sea(index) && !this->world.is_coast(index)) {
                claimed_spots.push_back(index);
                
                Region& region = this->world.world_map[index];
                Player& player = this->players[current_player]; 
                region.owner = &player;
                player.addOwnedRegion(index);
                player.team_colour = TEAM_COLOURS[current_player];

                spot_found = true;
                break;
            }
        }
        
        current_player++;
    }

    for(auto& player : this->players)
        this->world.world_map[player.getCapital()].object_colour = player.team_colour;

    this->players[0].setHuman(true);
    this->players[0].setCountryName("Sanity Wardens");
    
    for(auto& player : this->players) {
        const int capital_index = player.getCapital();
        auto&     region        = this->world.world_map[capital_index];

        Unit pawn = Unit();
        pawn.current_index = capital_index;
        pawn.object_size = sf::Vector2f(32, 16);
        pawn.object_texture_name = "unit_worldmap_settler";
        pawn.object_name = "settler";
        pawn.object_position = region.getPosition() + sf::Vector2f(16, 8);
        pawn.object_colour = COLOUR_RED;
        player.addUnit(pawn);

        // Assign the unit already stored, and not the temporary variable.  
        region.unit = player.getUnit(pawn.getID());
    }

    std::cout << "[Simulation Manager]: Starting locations found.\n";    
}

Player& SimulationManager::getHumanPlayer() {
    return this->players[0];
}

void SimulationManager::prepare() {
    this->world.forests   = std::map <int, GameObject> ();
    this->world.world_map = std::vector <Region> (this->world.getWorldSize());
    this->world.rivers    = std::map <int, GameObject> ();
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
std::vector <int> SimulationManager::astar(int start_index, int end_index) const {
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
    std::vector <int>       solution;

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
    
        if(counter >= ASTAR_LIMIT) {
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
                smallest_cost      = neighbour.cost_heuristic;
                index              = neighbour.index;
                neighbour.visited  = true;
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

void SimulationManager::updateScheduler() {
    // Update buildigns across all regions.
    
    auto& update_buildings = this->global_updates.at("update_buildings");
    if(update_buildings.first != update_buildings.second)
        update_buildings.first++;
    
    if(update_buildings.first ==  update_buildings.second) {
        this->updateBuildings();
        update_buildings.first = 0;
    }
}

void SimulationManager::updateBuildings() {
    for(int world_index = 0; world_index < this->world.world_map.size(); world_index++) {
        auto& region = this->world.world_map[world_index];

        if(region.buildings.size()) {
            for(auto& pair : region.buildings) {
                int   building_index = pair.first;  // Index of the tile on which a building stands.
                auto& building       = pair.second; // Building itself.

                switch(building.getNumericalType()) {
                    default:
                        break;

                    case 3: {
                        // Add 1 gold for each person living in the house.

                        region.addResource(ResourceType::RESOURCE_GOLD, 4);
                        break;
                    }

                    case 4: {
                        region.addResource(ResourceType::RESOURCE_FOOD, 10);
                        break;
                    }

                    case 5: {
                        // Quarry building.
                        // A quarry's efficiency depends on the number of stone tiles surrounding it.
                        
                        const int stone_scan_size = 3;
                        int       stone_tiles = 0;

                        for(int y = -stone_scan_size; y <= stone_scan_size; y++) {
                            for(int x = -stone_scan_size; x <= stone_scan_size; x++) {
                                const int index = building_index + y * this->settings.region_size + x;

                                if(region.map[index].object_texture_name == "tile_resource_stone")
                                    stone_tiles++;
                            }
                        }

                        region.addResource(ResourceType::RESOURCE_STONE, stone_tiles);
                        break;
                    }

                    case 6: {               
                        // Woodcutter building.
                        // A woodcutter's efficiency depends on the number of trees surrounding it.

                        const int tree_scan_size = 5;
                        int       trees = 0;
                        for(int y = -tree_scan_size; y <= tree_scan_size; y++) {
                            for(int x = -tree_scan_size; x <= tree_scan_size; x++) {
                                const int index = building_index + y * this->settings.region_size + x;
                                
                                if(region.trees.count(index))
                                    trees++;
                            }
                        }

                        Regionmap* regionmap = static_cast<Regionmap*> (this->gamestate.getGamestateByName("regionmap"));
                        regionmap->recalculateMesh();

                        region.addResource(ResourceType::RESOURCE_WOOD, trees);
                        break;
                    }
                }
            }
        }
    }
}
