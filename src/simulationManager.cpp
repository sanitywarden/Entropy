#include "simulationManager.hpp"
#include "entropy/gamestate.hpp"
#include "worldmap.hpp"
#include "regionmap.hpp"
#include "generationSettings.hpp"
#include "nameGenerator.hpp"
#include "tooltip.hpp"
#include "colours.hpp"
#include "luadriver.hpp"
#include "noiseSettings.hpp"

#include <queue>
#include <iostream>

using namespace iso;

SimulationManager::SimulationManager() 
    : Entropy(lua_driver.getApplicationSettings())
{
    this->initialise();
    this->texturizer = Texturizer(&this->resource);

    this->window.setTitle("Entropy by Vivit");
    this->window.setKeyHold(false);

    static Worldmap worldmap(this);
    this->gamestate.addGamestate("worldmap", worldmap);
    
    this->prepare();

    this->gamestate.setGamestate("worldmap");

    this->simulation_speed = game_settings.simulationSpeed();

    // Global updates.

    this->global_updates.insert({ "update_units",            std::pair(0, 1) }); // Move units.
    this->global_updates.insert({ "gui_tooltip_timer",       std::pair(0, 2) }); // Measure the time to display a tooltip.
    
    // Simulation updates.

    this->simulation_updates.insert({ "update_buildings",        std::pair(0, seconds_per_hour) }); // Update buildings in every region.
    this->simulation_updates.insert({ "update_population_needs", std::pair(0, seconds_per_day)  }); // Distribute resources to pops.
    this->simulation_updates.insert({ "event_timer",             std::pair(0, seconds_per_week) }); // Every week there should be a chance of a random event occuring.
    // this->simulation_updates.insert({ "update_quests",           std::pair(0, seconds_per_hour) }); // Update quests.
}

SimulationManager::~SimulationManager() 
{}

void SimulationManager::loop() {
    this->m_measurement_clock = sf::Clock();
    this->simulation_clock    = sf::Clock();

    const sf::Time one_second       = sf::seconds(1.0f);
    const sf::Time simulation_speed = sf::seconds(this->simulation_speed); 

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
            
            auto* gamestate = this->gamestate.getGamestate();
            if(this->time < INT_MAX && gamestate->state_id != "Menu")
                this->time++;

            this->updateSchedulerGlobal();

            updates = 0;
        }

        // Simulation updates.
        // Here execute updates depending on the simulation speed. 

        if(this->simulation_time_since_start.asMilliseconds() < simulation_speed.asMilliseconds()) {
            this->simulation_time_since_start = this->simulation_clock.getElapsedTime();
        }

        else {
            this->simulation_clock.restart();
            this->simulation_time_since_start = sf::Time::Zero;

            auto* gamestate = this->gamestate.getGamestate();
            if(this->simulation_time < INT_MAX && gamestate->state_id != "Menu")
                this->simulation_time++;

            // Update gamestate-specific scheduler.
            if(gamestate) {
                gamestate->updateScheduler();
            }

            // Update global scheduler.
            this->updateShedulerSimulation();
        }
    }
}

void SimulationManager::internalLoop(float delta_time) {
    Gamestate* gamestate = this->gamestate.getGamestate();
    if(gamestate == nullptr) {
        std::cout << "[Simulation Manager]: Gamestate is a nullptr.\n";
        this->exitApplication(1);
    } 

    this->emitEvents();

    // Player updates.
    gamestate->update(delta_time);
    gamestate->render(delta_time);

    this->popEvents();    
}

void SimulationManager::prepare() {
    this->initialise();
    this->world = WorldGenerator(&this->resource, &this->texturizer);
    this->world.world_map = std::vector <Region> (game_settings.getWorldSize());
    this->world.forests   = std::map <int, GameObject> ();
    this->world.rivers    = std::map <int, GameObject> ();
    this->world.lakes     = std::map <int, GameObject> ();
    this->players         = std::vector <Player> ();
    this->world.generateWorld();
    this->generateCountries();
}

void SimulationManager::updateShedulerSimulation() {
    // Update buildings across all regions.
    
    auto& update_buildings = this->simulation_updates.at("update_buildings");
    if(update_buildings.first != update_buildings.second)
        update_buildings.first++;
    
    if(update_buildings.first == update_buildings.second) {
        this->updateBuildings();
        update_buildings.first = 0;
    }

    // Update pops in regions.
    auto& update_pop_needs = this->simulation_updates.at("update_population_needs");
    if(update_pop_needs.first != update_pop_needs.second)
        update_pop_needs.first++;

    if(update_pop_needs.first == update_pop_needs.second) {
        this->updatePopulation();
        update_pop_needs.first = 0;
    }    

    auto& random_event_timer = this->simulation_updates.at("event_timer");
    if(random_event_timer.first != random_event_timer.second)
        random_event_timer.first++;

    if(random_event_timer.first == random_event_timer.second) {
        this->updateRandomEvent();
        random_event_timer.first = 0;
    }

    // Update quests.
    // auto& update_quests = this->simulation_updates.at("update_quests");
    // if(update_quests.first != update_quests.second)
    //     update_quests.first++;

    // if(update_quests.first == update_quests.second) {
    //     this->updateQuest();
    //     update_quests.first = 0;
    // }
}

void SimulationManager::updateSchedulerGlobal() {
    // Update units on the worldmap.

    auto& update_units = this->global_updates.at("update_units");
    if(update_units.first != update_units.second)
        update_units.first++;

    if(update_units.first == update_units.second) {
        this->updateUnits();
        update_units.first = 0;
    }

    // Update tooltip timer.

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

        /*
        if(region.buildings.size()) {
            for(auto& pair : region.buildings) {
                int   building_index = pair.first;        // Index of the tile on which a building stands.
                auto* building       = pair.second.get(); // Building itself.

                building->update(&region, building_index);
            }
        }
        */
    }
}

void SimulationManager::updateUnits() {
    // for(auto& player : this->players) {
    //     for(auto& unit : player.units) {
    //         if(unit.get()->hasPath()) {
    //             auto current   = unit.get()->current_index;
    //             auto next_move = unit.get()->getNextMove();
                
    //             const auto& region = this->world.world_map[next_move];
    //             unit.get()->object_position = region.getPosition();

    //             // TODO: This line will be a problem, because 2 units can not stand on the same tile,
    //             // but currently there is nothing that makes it impossible for two units to cross each other's paths. 
    //             this->world.world_map[next_move].unit = unit.get();
    //             this->world.world_map[current].unit   = nullptr;
    
    //             unit.get()->current_index = next_move;

    //             for(int y = -1; y <= 1; y++) {
    //                 for(int x = -1; x <= 1; x++) {
    //                     const int index = next_move + game_settings.calculateWorldIndex(x, y);

    //                     if(!player.discoveredRegion(index)) 
    //                         player.discovered_regions.push_back(index);
    //                 }
    //             }
    //         }
    //     }
    // }
}

void SimulationManager::updatePopulation() {
    // for(auto& region : this->world.world_map) {
    //     if(region.getPopulation() && region.visited && game_settings.populationNeedsEnabled()) {
    //         // Calculate people with their needs not fullfilled currently.

    //         const auto water_quantity      = region.getDrinkableLiquidQuantity();
    //         const auto pop_needs_met_water = water_quantity / water_consumed_per_pop;
    //         const auto water_needed        = region.getPopulation() * water_consumed_per_pop;
    //         const auto dehydrated_people   = region.getPopulation() - pop_needs_met_water <= 0
    //             ? 0
    //             : region.getPopulation() - pop_needs_met_water;
            
    //         const auto food_quantity       = region.getFoodQuantity();
    //         const auto pop_needs_met_food  = food_quantity / food_consumed_per_pop;
    //         const auto food_needed         = region.getPopulation() * food_consumed_per_pop;
    //         const auto malnourished_people = region.getPopulation() - pop_needs_met_food <= 0
    //             ? 0
    //             : region.getPopulation() - pop_needs_met_food; 


    //         // Calculate the number of dead people, including people with their needs not fullfilled from previous updates.
    //         // Select the smaller number.
    //         const auto dead_from_dehydration = this->people_dehydrated > dehydrated_people
    //             ? dehydrated_people
    //             : this->people_dehydrated;

    //         // Select the smaller number.
    //         const auto dead_from_malnutrition = this->people_malnourished > malnourished_people
    //             ? malnourished_people
    //             : this->people_malnourished;

    //         const auto dead_average = (dead_from_dehydration + dead_from_malnutrition) / 2;
    //         auto survive = region.getPopulation() - dead_average; 

    //         if(this->debugModeEnabled()) {
    //             std::cout << "=======[DEBUG]=======\n";
    //             std::cout << "[] Water needs met: " << pop_needs_met_water << " / " << region.getPopulation() << " people.\n";
    //             std::cout << "[] Food needs met:  " << pop_needs_met_food  << " / " << region.getPopulation() << " people.\n";
    //             std::cout << "[] Dehydrated currently:   " << dehydrated_people   << " | Dehydrated last time:   " << this->people_dehydrated   << "\n";
    //             std::cout << "[] Malnourished currently: " << malnourished_people << " | Malnourished last time: " << this->people_malnourished << "\n";
    //             std::cout << "[] Dead:    " << dead_average << "\n";
    //             std::cout << "[] Survive: " << survive      << "\n";
    //         }

    //         while(region.getPopulation() != survive) {
    //             region.population.resize(region.population.size() - 1);
    //         }

    //         const auto new_dehydrated = dehydrated_people - dead_from_dehydration;
    //             this->people_dehydrated = new_dehydrated;

    //         const auto new_malnourished = malnourished_people - dead_from_malnutrition;
    //             this->people_malnourished = new_malnourished;

    //         for(const auto& item : ITEM_LOOKUP_TABLE) {
    //             if(region.checkItemExists(item) && item.item_type == ItemType::TYPE_FOOD) {
    //                 // Find the food's share of all food. 
    //                 const auto percent_of_all_food = (float)region.getItemQuantity(item) / (float)food_quantity; 
    //                 const auto percent_rounded = std::ceil(percent_of_all_food * 100) / 100;

    //                 // Calculate the equivalent percentage of this food in the smaller quantity.
    //                 const int quantity = percent_rounded * food_needed;                                                     
    //                 const auto item_copy = StorageItem(item.item_name, -quantity, item.item_type);                                        
                    
    //                 if(this->debugModeEnabled())
    //                     std::cout << "[] " << item.item_name << " consumed:\t" << quantity << "\t(" << percent_rounded * 100 << "%)\n";
                    
    //                 region.addItem(item_copy);
    //             }

    //             if(region.checkItemExists(item) && item.item_type == ItemType::TYPE_DRINKABLE_LIQUID) {
    //                 // Find the liquid's share of all liquids.
    //                 const auto percent_of_all_liquids = (float)region.getItemQuantity(item) / (float)water_quantity;
    //                 const auto percent_rounded = std::ceil(percent_of_all_liquids * 100) / 100;
                    
    //                 // Calculate the equivalent percentage of this liquid in the smaller quantity.
    //                 const int quantity = percent_rounded * water_needed;
    //                 const auto item_copy = StorageItem(item.item_name, -quantity, item.item_type);

    //                 if(this->debugModeEnabled())
    //                     std::cout << "[] " << item.item_name << " consumed:\t" << quantity << "\t(" << percent_rounded * 100 << "%)\n";

    //                 region.addItem(item_copy);
    //             }
    //         }

    //         if(this->debugModeEnabled())
    //             std::cout << "=======[DEBUG]=======\n";
    //     }
    // }
}

void SimulationManager::updateRandomEvent() {
    
}

// void SimulationManager::updateQuest() {
    // for(const auto& player : this->players) {
    //     for(const auto& event : player.current_quests) {
    //         if(event.get()->isFinishConditionSatisfied()) {
    //             player.finishQuest(event);
    //             player.removeQuest(event);
    //         }
    //     }
    // }
// }

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

        auto index = game_settings.calculateWorldIndex(grid_position.x, grid_position.y);

        std::vector <int> neighbours;
        if(game_settings.inWorldBounds(grid_left))
            neighbours.push_back(index - 1);

        if(game_settings.inWorldBounds(grid_right))
            neighbours.push_back(index + 1);

        if(game_settings.inWorldBounds(grid_up))
            neighbours.push_back(index - game_settings.getWorldWidth());

        if(game_settings.inWorldBounds(grid_down))
            neighbours.push_back(index + game_settings.getWorldWidth());

        return neighbours;
    };

    const auto passable = [this](int index) -> bool {
        const auto& region = this->world.world_map[index];
        if(region.regiontype.is_ocean())
            return false;
        return true;
    };

    std::vector <aNode> nodes(game_settings.getWorldSize());
    
    

    for(int y = 0; y < game_settings.getWorldWidth(); y++) {
        for(int x = 0; x < game_settings.getWorldWidth(); x++) {
            const int index = game_settings.calculateWorldIndex(x, y);
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

        auto index = game_settings.calculateRegionIndex(grid_position.x, grid_position.y);

        std::vector <int> neighbours;
        
        if(game_settings.inRegionBounds(grid_left))
            if(region.isPassableRegionmap(game_settings.calculateRegionIndex(grid_left.x, grid_right.y)))
                neighbours.push_back(index - 1);

        if(game_settings.inRegionBounds(grid_right))
            if(region.isPassableRegionmap(game_settings.calculateRegionIndex(grid_right.x, grid_right.y)))
                neighbours.push_back(index + 1);

        if(game_settings.inRegionBounds(grid_up))
            if(region.isPassableRegionmap(game_settings.calculateRegionIndex(grid_up.x, grid_up.y)))
                neighbours.push_back(index - game_settings.getRegionWidth());

        if(game_settings.inRegionBounds(grid_down))
            if(region.isPassableRegionmap(game_settings.calculateRegionIndex(grid_down.x, grid_down.y)))
                neighbours.push_back(index + game_settings.getRegionWidth());

        return neighbours;
    };
    
    std::vector <aNode> nodes(game_settings.getRegionSize());
    auto& node_start = nodes[start];
    auto& node_end   = nodes[end];

    node_start.index = start;
    node_start.passable = region.isPassableRegionmap(start);

    node_end.index = end;
    node_end.passable = region.isPassableRegionmap(end);

    if(!node_end.passable) {
        std::cout << "[DEBUG][A*]: End node is unpassable.\n";
        return std::vector<int> ();
    }

    for(int y = 0; y < game_settings.getRegionWidth(); y++) {
        for(int x = 0; x < game_settings.getRegionWidth(); x++) {
            const int index = game_settings.calculateRegionIndex(x, y);
            aNode& node = nodes[index];

            node.index = index;        
            node.x = x;
            node.y = y;
            node.passable = region.isPassableRegionmap(index); 
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
    
        if(region.treeExistsAt(current.index))
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

    this->font_size = (this->window.windowWidth() + this->window.windowHeight()) / 160;
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

// Unit* SimulationManager::getUnit(int unit_id) {
//     for(Player& player : this->players) {
//         for(auto& unit_sp : player.units) {
//             auto* unit = unit_sp.get();
//             if(unit->getID() == unit_id)
//                 return unit;
//         }
//     }
//     return nullptr;
// }

void SimulationManager::generateCountries() {
    // Find spots suitable for settling. 

    std::vector <int> occupied_regions;

    auto number_of_players = game_settings.getPlayerQuantity();
    this->players.resize(number_of_players);
    
    for(int player_id = 0; player_id < number_of_players; player_id++) {
        auto settle_spot_found = false;
        auto settle_spot_index = -1;

        while(!settle_spot_found) {
            auto index = rand() % game_settings.getWorldSize();
            const auto& region = this->world.world_map[index];
            
            if(region.regiontype.is_terrain() && std::find(occupied_regions.begin(), occupied_regions.end(), index) == occupied_regions.end()) {
                settle_spot_found = true;    
                settle_spot_index = index;
                occupied_regions.push_back(index);
            }
        }

        auto& player = this->players[player_id];
        auto& region = this->world.world_map[settle_spot_index];

        // Not transparent colour, generated randomly.
        // The same colour will mark the player's regions on the worldmap, but it will have higher opacity.

        auto generated_colour_full  = this->texturizer.getRandomColour();
        auto generated_country_name = generate(GenerationType::COUNTRY, 3);
        
        player.setTeamColour(generated_colour_full);
        player.setCountryName(generated_country_name);

        // auto unit = this->createUnit(UnitType::UNIT_SETTLER, settle_spot_index, &player); 
        // if(unit)
        //     player.addUnit(unit);

        for(int y = -2; y <= 2; y++) {
            for(int x = -2; x <= 2; x++) {
                const int index = settle_spot_index + game_settings.calculateWorldIndex(x, y);
                player.discovered_regions.push_back(index);
            }
        }

        // region.unit = player.getUnit(unit.get()->getID());
        
        if(player_id == 0) {
            player.is_human = true;
            player.spawn_spot_index = settle_spot_index;
        }
    }
}

// std::shared_ptr <Unit> SimulationManager::createUnit(UnitType unit_type, int region_index, Player* owner) const {
//     return nullptr;

//     const auto& region = this->world.world_map[region_index];
    
//     // if(region.isUnitPresent()) {
//     //     std::cout << "[DEBUG]: Could not spawn unit on region " << region_index << ": Unit is already present.\n";
//     //     return nullptr;
//     // }

//     std::shared_ptr <Unit> unit;

//     switch(unit_type) {
//         default:
//             return nullptr;

//         case UnitType::UNIT_SETTLER: {
//             unit = std::shared_ptr <Unit> (new Unit("Settler"));
//             unit.get()->current_index = region_index;
//             unit.get()->object_texture_name = "unit_worldmap_settler";
            
//             break;
//         }
//     }

//     unit.get()->object_position = region.getPosition();
//     unit.get()->object_size     = region.getSize();
//     unit.get()->owner_id        = owner->getID();
//     unit.get()->type            = unit_type;

//     return unit;
// }

// void SimulationManager::deleteUnit(int unit_id) {
//     auto* unit = this->getUnit(unit_id);
//     if(!unit)
//         return;

//     if(game_settings.inWorldBounds(unit->current_index)) {
//         auto& region = this->world.world_map[unit->current_index];
//         region.unit = nullptr; 
//     }
    
//     auto* owner = this->getPlayer(unit->owner_id);
//     if(owner)
//         owner->removeUnit(unit_id); 

//     auto* worldmap = static_cast<Worldmap*>(this->gamestate.getGamestateByName("worldmap"));
//     if(worldmap)
//         worldmap->selected_unit_id = -1; 
// }

bool SimulationManager::regionCanBeColonised(int region_index) const {
    if(!game_settings.inWorldBounds(region_index))
        return false;
    
    const auto& region = this->world.world_map[region_index];
    if(region.isOwned())
        return false;
    
    if(region.regiontype.is_ocean())
        return false;
    return true;
}

void SimulationManager::emitEvents() {
    auto* gamestate = this->gamestate.getGamestate();
       
    std::map <std::string, sf::Keyboard::Key> keys = {
        { "A", sf::Keyboard::Key::A },
        { "B", sf::Keyboard::Key::B },
        { "C", sf::Keyboard::Key::C },
        { "D", sf::Keyboard::Key::D },
        { "E", sf::Keyboard::Key::E },
        { "F", sf::Keyboard::Key::F },
        { "G", sf::Keyboard::Key::G },
        { "H", sf::Keyboard::Key::H },
        { "I", sf::Keyboard::Key::I },
        { "J", sf::Keyboard::Key::J },
        { "K", sf::Keyboard::Key::K },
        { "L", sf::Keyboard::Key::L },
        { "M", sf::Keyboard::Key::M },
        { "N", sf::Keyboard::Key::N },
        { "O", sf::Keyboard::Key::O },
        { "P", sf::Keyboard::Key::P },
        { "R", sf::Keyboard::Key::R },
        { "S", sf::Keyboard::Key::S },
        { "T", sf::Keyboard::Key::T },
        { "U", sf::Keyboard::Key::U },
        { "W", sf::Keyboard::Key::W },
        { "V", sf::Keyboard::Key::V },
        { "Q", sf::Keyboard::Key::Q },
        { "X", sf::Keyboard::Key::X },
        { "Y", sf::Keyboard::Key::Y },
        { "Z", sf::Keyboard::Key::Z },

        { "0", sf::Keyboard::Key::Num0 },
        { "1", sf::Keyboard::Key::Num1 },
        { "2", sf::Keyboard::Key::Num2 },
        { "3", sf::Keyboard::Key::Num3 },
        { "4", sf::Keyboard::Key::Num4 },
        { "5", sf::Keyboard::Key::Num5 },
        { "6", sf::Keyboard::Key::Num6 },
        { "7", sf::Keyboard::Key::Num7 },
        { "8", sf::Keyboard::Key::Num8 },
        { "9", sf::Keyboard::Key::Num9 },

        { "F1", sf::Keyboard::Key::F1 },
        { "F2", sf::Keyboard::Key::F2 },
        { "F3", sf::Keyboard::Key::F3 },
        { "F4", sf::Keyboard::Key::F4 },
        { "F5", sf::Keyboard::Key::F5 },
        { "F6", sf::Keyboard::Key::F6 },
        { "F7", sf::Keyboard::Key::F7 },
        { "F8", sf::Keyboard::Key::F8 },
        { "F9", sf::Keyboard::Key::F9 },
        { "F10", sf::Keyboard::Key::F10 },
        { "F11", sf::Keyboard::Key::F11 },
        { "F12", sf::Keyboard::Key::F12 },

        { "ARROW_LEFT" , sf::Keyboard::Key::Left  },
        { "ARROW_RIGHT", sf::Keyboard::Key::Right },
        { "ARROW_UP"   , sf::Keyboard::Key::Up    },
        { "ARROW_DOWN" , sf::Keyboard::Key::Down  },

        { "SPACEBAR" , sf::Keyboard::Key::Space     },
        { "TAB"      , sf::Keyboard::Key::Tab       },
        { "ENTER"    , sf::Keyboard::Key::Enter     },
        { "BACKSPACE", sf::Keyboard::Key::Backspace },
        { "ESCAPE"   , sf::Keyboard::Key::Escape    },
        { "MENU"     , sf::Keyboard::Key::Menu      },

        // It does not matter that the mapped key is left.
        // It's overriden by the KeyPressed and KeyReleased event to check for both the keys.
        { "SHIFT" , sf::Keyboard::Key::LShift   },
        { "CTRL"  , sf::Keyboard::Key::LControl },
        { "ALT"   , sf::Keyboard::Key::LAlt     },
        { "SYSTEM", sf::Keyboard::Key::LSystem  },
    };

    if(gamestate->controls.addKeyMappingCheck("A", keys["A"])) {
        for(const auto& pair : keys) {
            gamestate->controls.addKeyMappingCheck(pair.first, pair.second);
        }
    }

    gamestate->controls.mouse_dragged = gamestate->controls.mouse_moved && gamestate->controls.mouse_middle;

    while(this->window.getWindow()->pollEvent(gamestate->event)) {
        switch(gamestate->event.type) {
            case sf::Event::Closed: {
                this->exitApplication(0);
                break;
            }

            case sf::Event::Resized: {
                event_queue.push_back("WINDOW_RESIZE");
                gamestate->controls.resized = sf::Vector2f(gamestate->event.size.width, gamestate->event.size.height);
                
                auto new_window_size = this->window.windowSize();
                this->font_size = (this->window.windowWidth() + this->window.windowHeight()) / 160;
            
                break;
            }

            case sf::Event::KeyPressed: {
                event_queue.push_back("BUTTON_PRESSED");
                
                for(const auto& pair : gamestate->controls.key_map) {
                    const auto& name  = pair.first;
                    const auto  state = sf::Keyboard::isKeyPressed(gamestate->controls.key_map[name]);

                    if(gamestate->controls.key_state.count(name)) {
                        gamestate->controls.key_state[name] = state;
                    }
                    else gamestate->controls.key_state.insert({ name, state });
                }
                
                break;
            }

            case sf::Event::KeyReleased: {
                event_queue.push_back("BUTTON_RELEASEED");
                
                for(const auto& pair : gamestate->controls.key_map) {
                    const auto& name  = pair.first;
                    const auto  state = sf::Keyboard::isKeyPressed(gamestate->controls.key_map[name]);

                    if(gamestate->controls.key_state.count(name)) {
                        gamestate->controls.key_state[name] = state;
                    }
                    else gamestate->controls.key_state.insert({ name, state });
                }
                
                break;
            }

            case sf::Event::MouseButtonPressed: {
                gamestate->controls.mouse_left   = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left); 
                gamestate->controls.mouse_right  = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right); 
                gamestate->controls.mouse_middle = sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle);

                if(gamestate->controls.mouse_middle)
                    gamestate->controls.button_position_pressed = gamestate->mouse_position_window;

                if(gamestate->controls.mouse_left)
                    for(const auto& pair : gamestate->interface)
                        pair.second->functionality();

                if(gamestate->controls.mouse_left)
                    event_queue.push_back("LMB_PRESSED");
                
                if(gamestate->controls.mouse_right)
                    event_queue.push_back("RMB_PRESSED");

                if(gamestate->controls.mouse_middle)
                    event_queue.push_back("MMB_PRESSED");
                
                break;
            }

            case sf::Event::MouseButtonReleased: {
                auto was_middle_pressed = gamestate->controls.mouse_middle;

                gamestate->controls.mouse_left   = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left); 
                gamestate->controls.mouse_right  = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right); 
                gamestate->controls.mouse_middle = sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle);

                gamestate->controls.mouse_dragged = false;

                if(was_middle_pressed && !gamestate->controls.mouse_middle)
                    gamestate->controls.button_position_released = gamestate->mouse_position_window;

                event_queue.push_back("MOUSE_BUTTON_RELEASED");
                break;
            }

            case sf::Event::MouseMoved: {
                gamestate->controls.mouse_moved = true;
                
                if(gamestate->controls.mouse_dragged)
                    gamestate->controls.button_position_released = gamestate->mouse_position_window;

                event_queue.push_back("MOUSE_MOVED");
                break;
            }

            case sf::Event::MouseWheelScrolled: {
                event_queue.push_back("MOUSE_WHEEL_SCROLLED");
                gamestate->controls.mouse_middle_down = gamestate->event.mouseWheelScroll.delta == -1;
                gamestate->controls.mouse_middle_up   = gamestate->event.mouseWheelScroll.delta == 1;
                break;
            }
        }
    }
}

void SimulationManager::popEvents() {
    if(event_queue.size())
        event_queue.erase(event_queue.begin());
}