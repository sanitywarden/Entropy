#include "simulationManager.hpp"
#include "entropy/gamestate.hpp"
#include "worldmap.hpp"
#include "regionmap.hpp"
#include "generationSettings.hpp"
#include "nameGenerator.hpp"
#include "menu.hpp"
#include "tooltip.hpp"

#include <iostream>

using namespace iso;

SimulationManager::SimulationManager() {
    this->initialise();
    this->texturizer = Texturizer(&this->resource);

    this->window.setTitle("Entropy by Vivit");
    this->window.setKeyHold(false);

    static Menu menu(this);
    this->gamestate.addGamestate("menu", menu);
    this->gamestate.setGamestate("menu");

    static Worldmap worldmap(this);
    this->gamestate.addGamestate("worldmap", worldmap);
    this->world = WorldGenerator(&this->resource, &this->texturizer);
    this->prepare();

    // Global updates.

    this->global_updates.insert({ "update_buildings",        std::pair(0, seconds_per_hour) }); // Update buildings in every region.
    this->global_updates.insert({ "update_units",            std::pair(0, 1) });                // Move units.
    this->global_updates.insert({ "update_population_needs", std::pair(0, seconds_per_day) });  // Distribute resources to pops.
    this->global_updates.insert({ "gui_tooltip_timer",       std::pair(0, 2) });                // Measure the time to display a tooltip.
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
            
            Gamestate* gamestate = this->gamestate.getGamestate();
            if(this->time < INT_MAX && gamestate->state_id != "Menu")
                this->time++;

            // Update gamestate-specific scheduler.
            if(gamestate) {
                gamestate->updateScheduler();
            }

            // Update global scheduler.
            this->updateScheduler();
            this->getDateFormatted();

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

void SimulationManager::prepare() {
    this->world.world_map = std::vector <Region> (world_settings.getWorldSize());
    this->world.forests   = std::map <int, GameObject> ();
    this->world.rivers    = std::map <int, GameObject> ();
    this->world.lakes     = std::map <int, GameObject> ();
    this->players         = std::vector <Player> ();
    this->world.generateWorld();
    this->initialiseWorld();

    auto* worldmap = static_cast<Worldmap*>(this->gamestate.getGamestateByName("worldmap"));
    if(worldmap)
        worldmap->centreOnCapital();
}

void SimulationManager::updateScheduler() {
    // Update buildings across all regions.
    
    auto& update_buildings = this->global_updates.at("update_buildings");
    if(update_buildings.first != update_buildings.second)
        update_buildings.first++;
    
    if(update_buildings.first == update_buildings.second) {
        this->updateBuildings();
        update_buildings.first = 0;
    }

    // Update units on the worldmap.

    auto& update_units = this->global_updates.at("update_units");
    if(update_units.first != update_units.second)
        update_units.first++;

    if(update_units.first == update_units.second) {
        this->updateUnits();
        update_units.first = 0;
    }

    // Update pops in regions.
    auto& update_pop_needs = this->global_updates.at("update_population_needs");
    if(update_pop_needs.first != update_pop_needs.second)
        update_pop_needs.first++;

    if(update_pop_needs.first == update_pop_needs.second) {
        this->updatePopulation();
        update_pop_needs.first = 0;
    }

    auto& gui_tooltip_timer = this->global_updates.at("gui_tooltip_timer");
    auto const* gamestate = this->gamestate.getGamestate();
    auto* interface_page = gamestate->getInterfaceComponent("component_tooltip");
    
    if(interface_page) {
        auto* tooltip = static_cast<gui::Tooltip*>(interface_page);
        if(!tooltip->intersectsSupportedUI()) {
            tooltip->show = false;
            gui_tooltip_timer.first = 0;
        }

        if(gui_tooltip_timer.first != gui_tooltip_timer.second && tooltip->intersectsSupportedUI())
            gui_tooltip_timer.first++;

        if(gui_tooltip_timer.first == gui_tooltip_timer.second && tooltip->intersectsSupportedUI()) {
            tooltip->show = true;
            gui_tooltip_timer.first = 0;
        }
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

void SimulationManager::updateUnits() {
    for(auto& player : this->players) {
        for(auto& unit : player.units) {
            if(unit.get()->hasPath()) {
                auto current   = unit.get()->current_index;
                auto next_move = unit.get()->getNextMove();
                
                const auto& region = this->world.world_map[next_move];
                unit.get()->object_position = region.getPosition();

                // TODO: This line will be a problem, because 2 units can not stand on the same tile,
                // but currently there is nothing that makes it impossible for two units to cross each other's paths. 
                this->world.world_map[next_move].unit = unit.get();
                this->world.world_map[current].unit   = nullptr;
    
                unit.get()->current_index = next_move;

                for(int y = -1; y <= 1; y++) {
                    for(int x = -1; x <= 1; x++) {
                        const int index = next_move + world_settings.calculateWorldIndex(x, y);

                        if(!player.discoveredRegion(index)) 
                            player.discovered_regions.push_back(index);
                    }
                }
            }
        }
    }
}

void SimulationManager::updatePopulation() {
    for(auto& region : this->world.world_map) {
        if(region.getPopulation() && region.visited && world_settings.populationNeedsEnabled()) {
            // Calculate people with their needs not fullfilled currently.

            const auto water_quantity      = region.getDrinkableLiquidQuantity();
            const auto pop_needs_met_water = water_quantity / water_consumed_per_pop;
            const auto water_needed        = region.getPopulation() * water_consumed_per_pop;
            const auto dehydrated_people   = region.getPopulation() - pop_needs_met_water <= 0
                ? 0
                : region.getPopulation() - pop_needs_met_water;
            
            const auto food_quantity       = region.getFoodQuantity();
            const auto pop_needs_met_food  = food_quantity / food_consumed_per_pop;
            const auto food_needed         = region.getPopulation() * food_consumed_per_pop;
            const auto malnourished_people = region.getPopulation() - pop_needs_met_food <= 0
                ? 0
                : region.getPopulation() - pop_needs_met_food; 


            // Calculate the number of dead people, including people with their needs not fullfilled from previous updates.
            // Select the smaller number.
            const auto dead_from_dehydration = this->people_dehydrated > dehydrated_people
                ? dehydrated_people
                : this->people_dehydrated;

            // Select the smaller number.
            const auto dead_from_malnutrition = this->people_malnourished > malnourished_people
                ? malnourished_people
                : this->people_malnourished;

            const auto dead_average = (dead_from_dehydration + dead_from_malnutrition) / 2;
            auto survive = region.getPopulation() - dead_average; 

            if(this->debugModeEnabled()) {
                std::cout << "=======[DEBUG]=======\n";
                std::cout << "[] Water needs met: " << pop_needs_met_water << " / " << region.getPopulation() << " people.\n";
                std::cout << "[] Food needs met:  " << pop_needs_met_food  << " / " << region.getPopulation() << " people.\n";
                std::cout << "[] Dehydrated currently:   " << dehydrated_people   << " | Dehydrated last time:   " << this->people_dehydrated   << "\n";
                std::cout << "[] Malnourished currently: " << malnourished_people << " | Malnourished last time: " << this->people_malnourished << "\n";
                std::cout << "[] Dead:    " << dead_average << "\n";
                std::cout << "[] Survive: " << survive      << "\n";
            }

            while(region.getPopulation() != survive) {
                region.population.resize(region.population.size() - 1);
            }

            const auto new_dehydrated = dehydrated_people - dead_from_dehydration;
                this->people_dehydrated = new_dehydrated;

            const auto new_malnourished = malnourished_people - dead_from_malnutrition;
                this->people_malnourished = new_malnourished;

            for(const auto& resource : RESOURCE_LOOKUP_TABLE) {
                if(region.checkResourceExists(resource) && resource.getType() == ResourceType::TYPE_FOOD) {
                    // Find the food's share of all food. 
                    const auto percent_of_all_food = (float)region.getResourceQuantity(resource) / (float)food_quantity; 
                    const auto percent_rounded = std::ceil(percent_of_all_food * 100) / 100;

                    // Calculate the equivalent percentage of this food in the smaller quantity.
                    const int quantity = percent_rounded * food_needed;                                                     
                    const auto resource_copy = Resource(resource.getName(), -quantity);                                        
                    
                    if(this->debugModeEnabled())
                        std::cout << "[] " << resource.getName() << " consumed:\t" << quantity << "\t(" << percent_rounded * 100 << "%)\n";
                    
                    region.addResource(resource_copy);
                }

                if(region.checkResourceExists(resource) && resource.getType() == ResourceType::TYPE_DRINKABLE_LIQUID) {
                    // Find the liquid's share of all liquids.
                    const auto percent_of_all_liquids = (float)region.getResourceQuantity(resource) / (float)water_quantity;
                    const auto percent_rounded = std::ceil(percent_of_all_liquids * 100) / 100;
                    
                    // Calculate the equivalent percentage of this liquid in the smaller quantity.
                    const int quantity = percent_rounded * water_needed;
                    const auto resource_copy = Resource(resource.getName(), -quantity);

                    if(this->debugModeEnabled())
                        std::cout << "[] " << resource.getName() << " consumed:\t" << quantity << "\t(" << percent_rounded * 100 << "%)\n";

                    region.addResource(resource_copy);
                }
            }

            if(this->debugModeEnabled())
                std::cout << "=======[DEBUG]=======\n";
        }
    }
}

struct aNode {
    int x;
    int y;
    int index;
    int h_cost;
    int g_cost;
    bool passable;
};

std::vector <int> SimulationManager::astar(int start, int end) const {
    const auto H = [](const aNode& node, const aNode& end) -> int {      
        return std::abs(end.x - node.x) + std::abs(end.y - node.y);
    };

    const auto neighbours = [this](sf::Vector2i grid_position) -> std::vector <int> {
        auto grid_up    = grid_position + sf::Vector2i(0, 1);
        auto grid_down  = grid_position + sf::Vector2i(0, -1);
        auto grid_left  = grid_position + sf::Vector2i(-1, 0);
        auto grid_right = grid_position + sf::Vector2i(1, 0);

        auto index = world_settings.calculateWorldIndex(grid_position.x, grid_position.y);

        std::vector <int> neighbours;
        if(world_settings.inWorldBounds(grid_left))
            neighbours.push_back(index - 1);

        if(world_settings.inWorldBounds(grid_right))
            neighbours.push_back(index + 1);

        if(world_settings.inWorldBounds(grid_up))
            neighbours.push_back(index - world_settings.getWorldWidth());

        if(world_settings.inWorldBounds(grid_down))
            neighbours.push_back(index + world_settings.getWorldWidth());

        return neighbours;
    };

    const auto passable = [this](int index) -> bool {
        const auto& region = this->world.world_map[index];
        if(region.regiontype.is_ocean())
            return false;

        auto* worldmap = static_cast<Worldmap*>(this->gamestate.getGamestate());
        auto* unit     = this->getUnit(worldmap->selected_unit_id);
        if(unit && region.unit) {
            if(*unit != *region.unit)
                return false;
        }

        return true;
    };

    std::vector <aNode> nodes(world_settings.getWorldSize());
    
    

    for(int y = 0; y < world_settings.getWorldWidth(); y++) {
        for(int x = 0; x < world_settings.getWorldWidth(); x++) {
            const int index = world_settings.calculateWorldIndex(x, y);
            aNode& node = nodes[index];

            node.index = index;        
            node.x = x;
            node.y = y;
            node.passable = passable(index); 
        }
    }

    const auto& node_start = nodes[start];
    const auto& node_end   = nodes[end];

    if(!node_end.passable)
        return std::vector<int> ();

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

    frontier.push(Intpair(H(node_start, node_end), node_start.index));
    cost_so_far[node_start.index] = 0;

    while(!frontier.empty()) {
        const aNode& current_node = nodes[(frontier.top()).second];
        frontier.pop();

        if(current_node.index == end)
            break;

        for(int index : neighbours(sf::Vector2i(current_node.x, current_node.y))) {
            const aNode& neighbour = nodes[index];
            const int new_cost   = cost_so_far[current_node.index] + 1;

            if(!neighbour.passable)
                continue;

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

std::vector <int> SimulationManager::r_astar(int start, int end) const {
    auto* gamestate = this->gamestate.getGamestateByName("regionmap");
    auto* regionmap = static_cast<Regionmap*>(gamestate);
    auto& region = this->world.world_map[regionmap->getRegionIndex()];

    const auto H = [](const aNode& node, const aNode& end) -> int {
        // Euclidean distance.
        // return std::sqrt(
        //     std::pow(node.x - end.x, 2) + std::pow(node.y - end.y, 2)
        // );

        // Manhattan distance.  
        return std::abs(end.x - node.x) + std::abs(end.y - node.y);
    };

    const auto G = [H](const aNode& start_node, const aNode& current_node) -> int {
        return H(start_node, current_node);
    };

    const auto neighbours = [region, this](sf::Vector2i grid_position) -> std::vector <int> {
        auto grid_up    = grid_position + sf::Vector2i(0, 1);
        auto grid_down  = grid_position + sf::Vector2i(0, -1);
        auto grid_left  = grid_position + sf::Vector2i(-1, 0);
        auto grid_right = grid_position + sf::Vector2i(1, 0);

        auto index = world_settings.calculateRegionIndex(grid_position.x, grid_position.y);

        std::vector <int> neighbours;
        
        if(world_settings.inRegionBounds(grid_left))
            if(region.isPassableAStar(world_settings.calculateRegionIndex(grid_left.x, grid_right.y)))
                neighbours.push_back(index - 1);

        if(world_settings.inRegionBounds(grid_right))
            if(region.isPassableAStar(world_settings.calculateRegionIndex(grid_right.x, grid_right.y)))
                neighbours.push_back(index + 1);

        if(world_settings.inRegionBounds(grid_up))
            if(region.isPassableAStar(world_settings.calculateRegionIndex(grid_up.x, grid_up.y)))
                neighbours.push_back(index - world_settings.getRegionWidth());

        if(world_settings.inRegionBounds(grid_down))
            if(region.isPassableAStar(world_settings.calculateRegionIndex(grid_down.x, grid_down.y)))
                neighbours.push_back(index + world_settings.getRegionWidth());

        return neighbours;
    };
    
    std::vector <aNode> nodes(world_settings.getRegionSize());
    auto& node_start = nodes[start];
    auto& node_end   = nodes[end];

    node_start.index = start;
    node_start.passable = region.isPassableAStar(start);

    node_end.index = end;
    node_end.passable = region.isPassableAStar(end);

    if(!node_end.passable) {
        std::cout << "[DEBUG][A*]: End node is unpassable.\n";
        return std::vector<int> ();
    }

    for(int y = 0; y < world_settings.getRegionWidth(); y++) {
        for(int x = 0; x < world_settings.getRegionWidth(); x++) {
            const int index = world_settings.calculateRegionIndex(x, y);
            aNode& node = nodes[index];

            node.index = index;        
            node.x = x;
            node.y = y;
            node.passable = region.isPassableAStar(index); 
            node.h_cost = H(node, node_end);
            node.g_cost = G(node_start, node);
        }
    }

    typedef std::pair<int, int> Intpair;
    typedef std::priority_queue <Intpair, std::vector <Intpair>, std::greater <Intpair>> Frontier;

    Frontier frontier;
    std::vector <int> seen;
    std::unordered_map <int, int> cost_so_far; // index, cost
    std::unordered_map <int, int> came_from;   // index, index

    frontier.push(Intpair(H(node_start, node_end), node_start.index));
    cost_so_far[node_start.index] = 0;

    const auto get_node_movement_cost = [region](const aNode& current, const aNode& next) -> int {
        if(region.map[current.index].tiletype.is_river())
            return 3;
    
        if(region.isTree(current.index))
            return 5;

        return 1;
    };

    const auto reconstruct_path = [](const aNode& start, const aNode& end, std::unordered_map <int, int> came_from) -> std::vector <int> {
        std::vector <int> path;
        int current_index = end.index;
        while(current_index != start.index) {
            path.push_back(current_index);
            current_index = came_from[current_index];
        }

        path.push_back(start.index);
        std::reverse(path.begin(), path.end());
        return path;
    };

    while(!frontier.empty()) {
        const auto& current_node = nodes[(frontier.top()).second];
        frontier.pop();
        
        if(current_node.index == end)
            return reconstruct_path(node_start, node_end, came_from);

        seen.push_back(current_node.index);

        auto current_neighbours = neighbours(sf::Vector2i(current_node.x, current_node.y));
        for(auto index : current_neighbours) {
            // If index was already seen, skip.
            if(std::find(seen.begin(), seen.end(), index) != seen.end())
                continue;

            const auto& neighbour = nodes[index];
            if(!neighbour.passable)
                continue;

            auto new_cost = cost_so_far[current_node.index] + get_node_movement_cost(current_node, neighbour);
            if(cost_so_far.find(index) == cost_so_far.end() || new_cost < cost_so_far[index]) {
                cost_so_far[index] = new_cost;
                auto priority = new_cost + H(neighbour, node_end);
                frontier.push({ priority, index });
                came_from[index] = current_node.index;
            }
        }
    }

    return reconstruct_path(node_start, node_end, came_from);
}

void SimulationManager::initialise() {
    this->draw_calls          = 0;
    this->people_dehydrated   = 0;
    this->people_malnourished = 0;
    this->resource.loadTexture("./res/random_colour.png", "random_colour");

    this->resource.loadTexture("./res/ui/template/template.png", "widget_base_top_left",                sf::IntRect(0, 0, 64, 64    ));
    this->resource.loadTexture("./res/ui/template/template.png", "widget_base_top_right",               sf::IntRect(128, 0, 64, 64  ));
    this->resource.loadTexture("./res/ui/template/template.png", "widget_base_bottom_left",             sf::IntRect(0, 128, 64, 64  ));
    this->resource.loadTexture("./res/ui/template/template.png", "widget_base_bottom_right",            sf::IntRect(128, 128, 64, 64));
    this->resource.loadTexture("./res/ui/template/template.png", "widget_base_middle",                  sf::IntRect(64, 64, 64, 64  ));
    this->resource.loadTexture("./res/ui/template/template.png", "widget_base_top",                     sf::IntRect(64, 0, 64, 64   ));
    this->resource.loadTexture("./res/ui/template/template.png", "widget_base_left",                    sf::IntRect(0, 64, 64, 64   ));
    this->resource.loadTexture("./res/ui/template/template.png", "widget_base_right",                   sf::IntRect(128, 64, 64, 64 ));
    this->resource.loadTexture("./res/ui/template/template.png", "widget_base_bottom",                  sf::IntRect(64, 128, 64, 64 ));
    this->resource.loadTexture("./res/ui/template/template.png", "widget_base_small_horizontal_left",   sf::IntRect(0, 192, 64, 64  ));
    this->resource.loadTexture("./res/ui/template/template.png", "widget_base_small_horizontal_right",  sf::IntRect(128, 192, 64, 64));
    this->resource.loadTexture("./res/ui/template/template.png", "widget_base_small_horizontal_middle", sf::IntRect(64, 192, 64, 64 ));
    this->resource.loadTexture("./res/ui/template/template.png", "widget_base_small_vertical_top",      sf::IntRect(192, 0, 64, 64  ));
    this->resource.loadTexture("./res/ui/template/template.png", "widget_base_small_vertical_bottom",   sf::IntRect(192, 128, 64, 64));
    this->resource.loadTexture("./res/ui/template/template.png", "widget_base_small_vertical_middle",   sf::IntRect(192, 64, 64, 64 ));
    this->resource.loadTexture("./res/ui/template/template.png", "widget_base_single",                  sf::IntRect(192, 192, 64, 64));

    this->resource.loadTexture("./res/ui/template/button.png",  "button_base_top_left",                sf::IntRect(0, 0, 8, 8      ));
    this->resource.loadTexture("./res/ui/template/button.png",  "button_base_top_right",               sf::IntRect(16, 0, 8, 8     ));
    this->resource.loadTexture("./res/ui/template/button.png",  "button_base_bottom_left",             sf::IntRect(0, 16, 8, 8     ));
    this->resource.loadTexture("./res/ui/template/button.png",  "button_base_bottom_right",            sf::IntRect(16, 16, 8, 8    ));
    this->resource.loadTexture("./res/ui/template/button.png",  "button_base_top",                     sf::IntRect(8, 0, 8, 8      ));
    this->resource.loadTexture("./res/ui/template/button.png",  "button_base_left",                    sf::IntRect(0, 8, 8, 8      ));
    this->resource.loadTexture("./res/ui/template/button.png",  "button_base_right",                   sf::IntRect(16, 8, 8, 8     ));
    this->resource.loadTexture("./res/ui/template/button.png",  "button_base_bottom",                  sf::IntRect(8, 16, 8, 8     ));
    this->resource.loadTexture("./res/ui/template/button.png",  "button_base_middle",                  sf::IntRect(8, 8, 8, 8      ));
    this->resource.loadTexture("./res/ui/template/button.png",  "button_base_small_horizontal_left",   sf::IntRect(0, 24, 8, 8     ));
    this->resource.loadTexture("./res/ui/template/button.png",  "button_base_small_horizontal_right",  sf::IntRect(16, 24, 8, 8    ));
    this->resource.loadTexture("./res/ui/template/button.png",  "button_base_small_horizontal_middle", sf::IntRect(8, 24, 8, 8     ));
    this->resource.loadTexture("./res/ui/template/button.png",  "button_base_small_vertical_top",      sf::IntRect(24, 0, 8, 8     ));
    this->resource.loadTexture("./res/ui/template/button.png",  "button_base_small_vertical_bottom",   sf::IntRect(24, 16, 8, 8    ));
    this->resource.loadTexture("./res/ui/template/button.png",  "button_base_small_vertical_middle",   sf::IntRect(24, 8, 8, 8     ));
    this->resource.loadTexture("./res/ui/template/button.png",  "button_base_single",                  sf::IntRect(24, 24, 8, 8    ));

    this->resource.loadTexture("./res/ui/template/template_black.png", "widget_black_base_top_left",                sf::IntRect(0, 0, 64, 64    ));
    this->resource.loadTexture("./res/ui/template/template_black.png", "widget_black_base_top_right",               sf::IntRect(128, 0, 64, 64  ));
    this->resource.loadTexture("./res/ui/template/template_black.png", "widget_black_base_bottom_left",             sf::IntRect(0, 128, 64, 64  ));
    this->resource.loadTexture("./res/ui/template/template_black.png", "widget_black_base_bottom_right",            sf::IntRect(128, 128, 64, 64));
    this->resource.loadTexture("./res/ui/template/template_black.png", "widget_black_base_middle",                  sf::IntRect(64, 64, 64, 64  ));
    this->resource.loadTexture("./res/ui/template/template_black.png", "widget_black_base_top",                     sf::IntRect(64, 0, 64, 64   ));
    this->resource.loadTexture("./res/ui/template/template_black.png", "widget_black_base_left",                    sf::IntRect(0, 64, 64, 64   ));
    this->resource.loadTexture("./res/ui/template/template_black.png", "widget_black_base_right",                   sf::IntRect(128, 64, 64, 64 ));
    this->resource.loadTexture("./res/ui/template/template_black.png", "widget_black_base_bottom",                  sf::IntRect(64, 128, 64, 64 ));
    this->resource.loadTexture("./res/ui/template/template_black.png", "widget_black_base_small_horizontal_left",   sf::IntRect(0, 192, 64, 64  ));
    this->resource.loadTexture("./res/ui/template/template_black.png", "widget_black_base_small_horizontal_right",  sf::IntRect(128, 192, 64, 64));
    this->resource.loadTexture("./res/ui/template/template_black.png", "widget_black_base_small_horizontal_middle", sf::IntRect(64, 192, 64, 64 ));
    this->resource.loadTexture("./res/ui/template/template_black.png", "widget_black_base_small_vertical_top",      sf::IntRect(192, 0, 64, 64  ));
    this->resource.loadTexture("./res/ui/template/template_black.png", "widget_black_base_small_vertical_bottom",   sf::IntRect(192, 128, 64, 64));
    this->resource.loadTexture("./res/ui/template/template_black.png", "widget_black_base_small_vertical_middle",   sf::IntRect(192, 64, 64, 64 ));
    this->resource.loadTexture("./res/ui/template/template_black.png", "widget_black_base_single",                  sf::IntRect(192, 192, 64, 64));

    this->resource.loadTexture("./res/ui/template/button_black.png",  "button_black_base_top_left",                sf::IntRect(0, 0, 8, 8      ));
    this->resource.loadTexture("./res/ui/template/button_black.png",  "button_black_base_top_right",               sf::IntRect(16, 0, 8, 8     ));
    this->resource.loadTexture("./res/ui/template/button_black.png",  "button_black_base_bottom_left",             sf::IntRect(0, 16, 8, 8     ));
    this->resource.loadTexture("./res/ui/template/button_black.png",  "button_black_base_bottom_right",            sf::IntRect(16, 16, 8, 8    ));
    this->resource.loadTexture("./res/ui/template/button_black.png",  "button_black_base_top",                     sf::IntRect(8, 0, 8, 8      ));
    this->resource.loadTexture("./res/ui/template/button_black.png",  "button_black_base_left",                    sf::IntRect(0, 8, 8, 8      ));
    this->resource.loadTexture("./res/ui/template/button_black.png",  "button_black_base_right",                   sf::IntRect(16, 8, 8, 8     ));
    this->resource.loadTexture("./res/ui/template/button_black.png",  "button_black_base_bottom",                  sf::IntRect(8, 16, 8, 8     ));
    this->resource.loadTexture("./res/ui/template/button_black.png",  "button_black_base_middle",                  sf::IntRect(8, 8, 8, 8      ));
    this->resource.loadTexture("./res/ui/template/button_black.png",  "button_black_base_small_horizontal_left",   sf::IntRect(0, 24, 8, 8     ));
    this->resource.loadTexture("./res/ui/template/button_black.png",  "button_black_base_small_horizontal_right",  sf::IntRect(16, 24, 8, 8    ));
    this->resource.loadTexture("./res/ui/template/button_black.png",  "button_black_base_small_horizontal_middle", sf::IntRect(8, 24, 8, 8     ));
    this->resource.loadTexture("./res/ui/template/button_black.png",  "button_black_base_small_vertical_top",      sf::IntRect(24, 0, 8, 8     ));
    this->resource.loadTexture("./res/ui/template/button_black.png",  "button_black_base_small_vertical_bottom",   sf::IntRect(24, 16, 8, 8    ));
    this->resource.loadTexture("./res/ui/template/button_black.png",  "button_black_base_small_vertical_middle",   sf::IntRect(24, 8, 8, 8     ));
    this->resource.loadTexture("./res/ui/template/button_black.png",  "button_black_base_single",                  sf::IntRect(24, 24, 8, 8    ));

    this->resource.loadFont("./res/font/proggy.ttf",   "proggy");
    this->resource.loadFont("./res/font/garamond.ttf", "garamond");
}

void SimulationManager::initialiseWorld() {
    // Find spots suitable for settling.
    // Spawn players.   

    std::vector <int> occupied_regions;

    // In the future this setting could be set by the human player.
    const int number_of_players = world_settings.getPlayerQuantity();
    this->players.resize(number_of_players);
    for(int player_id = 0; player_id < number_of_players; player_id++) {
        bool settle_spot_found = false;
        int  settle_spot_index = -1;

        while(!settle_spot_found) {
            int index = rand() % world_settings.getWorldSize();
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
        const auto generated_colour_full  = this->texturizer.getRandomColour();
        const auto generated_country_name = generate(GenerationType::COUNTRY, 3);
        
        auto unit = std::shared_ptr <Unit> (new Unit("Settler"));
        unit.get()->object_texture_name = "unit_worldmap_settler";
        unit.get()->object_size         = region.getSize();
        unit.get()->object_position     = region.getPosition();
        unit.get()->current_index       = settle_spot_index;
        unit.get()->owner_id            = player_id;
        player.addUnit(unit);

        for(int y = -2; y <= 2; y++) {
            for(int x = -2; x <= 2; x++) {
                const int index = settle_spot_index + world_settings.calculateWorldIndex(x, y);
                player.discovered_regions.push_back(index);
            }
        }

        region.unit = player.getUnit(unit.get()->getID());

        player.setTeamColour(generated_colour_full);
        player.setCountryName(generated_country_name);
        
        if(player_id == 0)
            player.is_human = true;
    }
}

int SimulationManager::getDrawCalls() const {
    return this->draw_calls;
}

void SimulationManager::updateDrawCalls(int calls) {
    this->draw_calls = calls;
}

std::string SimulationManager::getDateFormatted() const {
    // Copy value.
    int time_passed = this->time;

    /* Time scale:
        12 irl seconds = 1 ig hour
        12 ig hours    = 1 ig day
        30 ig days     = 1 ig month
        12 ig months   = 1 ig year
    */

    int seconds_left = 0;
    int years = time_passed / seconds_per_year;
    seconds_left = time_passed % seconds_per_year;
 
    int months = seconds_left / seconds_per_month;
    seconds_left = seconds_left % seconds_per_month;
  
    int days = seconds_left / seconds_per_day;
    seconds_left = seconds_left % seconds_per_day;

    std::string date;
    date += std::to_string(days)   + " "; 
    date += std::to_string(months) + " "; 
    date += std::to_string(years); 

    // Example date (D/M/Y): "11 12 54".
    return date;
}

bool SimulationManager::inScreenSpace(const GameObject& object) const {
    auto* gamestate = this->gamestate.getGamestate();
    auto  view = gamestate->view_game;
    auto  view_centre = view.getCenter();
    auto  view_size   = view.getSize();
    sf::Rect view_box(view_centre - sf::Vector2f(view_size.x / 2, view_size.y / 2), view_size);
    sf::Rect object_box(object.getPosition2D(), object.getSize());
    
    return view_box.intersects(object_box);
}

Player* SimulationManager::getHumanPlayer() {
    for(auto& player : this->players)
        if(player.isHuman())
            return &player;
    return nullptr;
}

Player* SimulationManager::getPlayer(int player_id) {
    for(auto& player : this->players)
        if(player.getID() == player_id)
            return &player;
    return nullptr;
}

bool SimulationManager::isHumanPlayer(int player_id) const {
    return this->getHumanPlayer()->getID() == player_id;
}

Unit* SimulationManager::getUnit(int unit_id) {
    for(Player& player : this->players) {
        for(auto& unit_sp : player.units) {
            auto* unit = unit_sp.get();
            if(unit->getID() == unit_id)
                return unit;
        }
    }
    return nullptr;
}
