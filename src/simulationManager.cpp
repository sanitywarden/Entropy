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
    settings.world_panel_size.x         = 128; // TODO: Panel size should be an int, not Vector2f.
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
    settings.region_size                = settings.world_panel_size.x;
    settings.region_tile_size.x         = 64;           
    settings.region_tile_size.y         = 32;           
    settings.region_tile_offset.x       = 100; 
    settings.region_tile_offset.y       = 0; 

    this->settings = settings;
    // Settings for world generation set.

    this->initialise();
    this->texturizer = Texturizer(&this->resource);

    this->window.setTitle("Entropy by Vivit");
    this->window.setKeyHold(false);

    static Worldmap worldmap = Worldmap(this);
    this->gamestate.addGamestate("worldmap", worldmap);
    this->world = WorldGenerator(&this->resource, &this->texturizer, this->settings);
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

Player& SimulationManager::getHumanPlayer() {
    return this->players[0];
}

void SimulationManager::prepare() {
    this->world.world_map = std::vector <Region> (this->world.getWorldSize());
    this->world.forests   = std::map <int, GameObject> ();
    this->world.rivers    = std::map <int, GameObject> ();
    this->world.lakes     = std::map <int, GameObject> ();
    this->world.generateWorld();
    this->initialiseWorld();
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
                int   building_index = pair.first;        // Index of the tile on which a building stands.
                auto* building       = pair.second.get(); // Building itself.

                building->update(&region, building_index);
            }
        }
    }
}

struct aNode {
    int x;
    int y;
    int index;
    int h_cost;
    bool passable;
};

std::vector <int> SimulationManager::astar(int start, int end) const {
    const auto H = [](const aNode& node, const aNode& end) -> int {
        return std::abs(end.x - node.x) + std::abs(end.y - node.y);
    };

    const int world_size = this->settings.world_size;
    const auto neighbours = [world_size](int index) -> std::vector <int> {
        std::vector <int> neighbours;
        if(index - 1 >= 0)
            neighbours.push_back(index - 1);

        if(index + 1 < world_size * world_size)
            neighbours.push_back(index + 1);

        if(index - world_size >= 0)
            neighbours.push_back(index - world_size);

        if(index + world_size < world_size * world_size)
            neighbours.push_back(index + world_size);

        return neighbours;
    };

    const auto passable = [this](int index) -> bool {
        const auto& region = this->world.world_map[index];
        return !region.regiontype.is_ocean();
    };

    std::vector <aNode> nodes(this->settings.world_size * this->settings.world_size);
    
    const auto& node_start = nodes[start];
    const auto& node_end   = nodes[end];

    for(int y = 0; y < this->settings.world_size; y++) {
        for(int x = 0; x < this->settings.world_size; x++) {
            const int index = y * this->settings.world_size + x;
            aNode& node = nodes[index];

            node.index = index;        
            node.x = x;
            node.y = y;
            node.passable = passable(index); 
        }
    }

    typedef std::pair<int, int> Intpair;
    typedef std::priority_queue <Intpair, std::vector <Intpair>, std::greater <Intpair>> Frontier;

    Frontier frontier;
    std::vector <int> seen;
    std::unordered_map <int, int> cost_so_far; // index, cost
    std::unordered_map <int, int> came_from;   // index, index

    const auto reconstruct = [](int start, int end, std::unordered_map <int, int> came_from) -> std::vector <int> {
        std::vector <int> path;
        int current = end;
        while(current != start) {
            path.push_back(current);
            current = came_from[current];
        }

        path.push_back(start);
        std::reverse(path.begin(), path.end());
        return path;
    };

    frontier.push(Intpair(node_start.h_cost, node_start.index));
    cost_so_far[node_start.index] = 0;

    while(!frontier.empty()) {
        const aNode& current_node = nodes[(frontier.top()).second];
        frontier.pop();

        if(current_node.index == end)
            break;

        for(int index : neighbours(current_node.index)) {
            const aNode& neighbour = nodes[index];
            const int new_cost   = cost_so_far[current_node.index] + 1;

            if(!neighbour.passable)
                continue;;

            if(cost_so_far.find(index) == cost_so_far.end() || new_cost < cost_so_far[index]) {
                cost_so_far[index] = new_cost;
                came_from[index]   = current_node.index;
                
                const int priority = new_cost + H(neighbour, node_end);
                frontier.push(Intpair(priority, index));
            }
        }
    }

    return reconstruct(start, end, came_from);
}

void SimulationManager::initialise() {
    this->draw_calls = 0;

    this->resource.loadTexture("./res/random_colour.png", "random_colour");
}

void SimulationManager::initialiseWorld() {
    // Find spots suitable for settling.
    // Spawn players.

    std::vector <int> occupied_regions;

    // In the future this setting could be set by the human player.
    const int number_of_players = this->world.settings.world_size / 10;
    this->players.resize(number_of_players);
    for(int player_id = 0; player_id < number_of_players; player_id++) {
        bool settle_spot_found = false;
        int  settle_spot_index = -1;

        while(!settle_spot_found) {
            int index = rand() % this->world.getWorldSize();
            const auto& region = this->world.world_map[index];
            if(region.regiontype.is_terrain() && std::find(occupied_regions.begin(), occupied_regions.end(), index) == occupied_regions.end()) {
                settle_spot_found = true;    
                settle_spot_index = index;
                occupied_regions.push_back(index);
            }
        }

        auto& player = this->players[player_id];
        auto& region = this->world.world_map[settle_spot_index];

        // Temporary colour, generated randomly.
        // In the future it could maybe be less random. That would better it's visibility.
        const sf::Color generated_colour_full    = this->texturizer.getRandomColour();
        const sf::Color colour_part_transparent  = sf::Color(generated_colour_full.r, generated_colour_full.g, generated_colour_full.b, 127);
        const std::string generated_country_name = "Country" + std::to_string(player_id);

        region.owner = &player;

        player.setCapital(settle_spot_index);
        player.addOwnedRegion(settle_spot_index);
        player.setTeamColour(colour_part_transparent);
        player.setCountryName(generated_country_name);
    }
}

int SimulationManager::getDrawCalls() const {
    return this->draw_calls;
}

void SimulationManager::updateDrawCalls(int calls) {
    this->draw_calls = calls;
}