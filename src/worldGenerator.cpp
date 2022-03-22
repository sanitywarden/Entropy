#include "worldGenerator.hpp"
#include "globalutilities.hpp"

using namespace iso;

WorldGenerator::WorldGenerator() {

}

WorldGenerator::WorldGenerator(entropy::resourceManager* resource, GenerationSettings& settings) {
    this->settings = settings;
    this->resource = resource;

    auto world_size = this->getWorldSize();

    this->m_gradient.resize(world_size);
    this->world_map.resize(world_size);

    std::srand(std::time(0));
}

WorldGenerator::~WorldGenerator() {

}

void WorldGenerator::generateWorld() {
    std::cout << "[World Generation]: Generating world.\n";

    this->m_region = Region();

    sf::Clock clock;

    // World size is x * y.
    const int WORLD_SIZE = this->getWorldSize();

    std::vector <float> worldmap_noise(WORLD_SIZE);

    // Generate initial noise map.
    {
        std::vector <float> input(WORLD_SIZE);
        for(int i = 0; i < WORLD_SIZE; i++) {
            input[i] = (float)rand() / (float)RAND_MAX;    
        }

        float biggest_noise_recorded = 1.0f;

        for(int y = 0; y < this->settings.world_size; y++) {
            for(int x = 0; x < this->settings.world_size; x++) {
                const int index = this->wCalculateIndex(x, y);

                if(x < this->settings.world_margin_island || x > (this->settings.world_size - this->settings.world_margin_island) || y < this->settings.world_margin_island || y > (this->settings.world_size - this->settings.world_margin_island)) {
                    worldmap_noise[index] = 0.0f;
                    continue;
                }

                float noise = 0.0f;
                float scale = 1.0f;
                float scale_acc = 0.0f;

                for(unsigned int o = 0; o < this->settings.world_noise_octaves; o++) {
                    const int sampleX1 = (x / this->settings.world_noise_persistence) * this->settings.world_noise_persistence;
                    const int sampleY1 = (y / this->settings.world_noise_persistence) * this->settings.world_noise_persistence;

                    const int sampleX2 = (sampleX1 + this->settings.world_noise_persistence) % this->settings.world_size;					
                    const int sampleY2 = (sampleY1 + this->settings.world_noise_persistence) % this->settings.world_size;

                    const float blendX = (float)(x - sampleX1) / (float)this->settings.world_noise_persistence;
                    const float blendY = (float)(y - sampleY1) / (float)this->settings.world_noise_persistence;

                    const float sampleT = (1.0f - blendX) * input[sampleY1 * this->settings.world_size + sampleX1] + blendX * input[sampleY1 * this->settings.world_size + sampleX2];
                    const float sampleB = (1.0f - blendX) * input[sampleY2 * this->settings.world_size + sampleX1] + blendX * input[sampleY2 * this->settings.world_size + sampleX2];

                    scale_acc += scale;
                    noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
                    scale = float(scale / this->settings.world_noise_bias);
                }
                
                float noise_value = (noise / scale_acc) * this->settings.world_noise_multiplier;
                
                if(noise_value > biggest_noise_recorded) biggest_noise_recorded = noise_value;

                noise_value *= biggest_noise_recorded;
                if(noise_value > 1.0f) noise_value = 1.0f;

                worldmap_noise[index] = noise_value;
            }
        }
    }

    // Generate circular gradient.
    // When combining noise with this gradient the output will be island shaped terrain.
    {
        const sf::Vector2i centre = sf::Vector2i(
            this->settings.world_size / 2,
            this->settings.world_size / 2
        );

        for(int y = 0; y < this->settings.world_size; y++) {
            for(int x = 0; x < this->settings.world_size; x++) {
                const int index = this->wCalculateIndex(x, y);

                if(x < this->settings.world_margin_island || x > (this->settings.world_size - this->settings.world_margin_island) || y < this->settings.world_margin_island || y > (this->settings.world_size - this->settings.world_margin_island)) {
                    this->m_gradient[index] = 0.0f;
                    continue;
                }

                const float distance_x = (centre.x - x) * (centre.x - x);
                const float distance_y = (centre.y - y) * (centre.y - y);

                // Both width and height of the map have a impact on distance.
                float distance = (sqrt(distance_x + distance_y) / this->settings.world_size);

                if(distance > 1.0f) distance = 1.0f;

                // Distance is bigger when further away from the centre of the gradient.
                this->m_gradient[index] = distance * this->settings.world_gradient_multiplier;
            }
        }
    }

    // Generate initial worldmap divided into water tiles and terrain tiles.
    for(int y = 0; y < this->settings.world_size; y++) {
        for(int x = 0; x < this->settings.world_size; x++) {                
            const int index      = this->wCalculateIndex(x, y);
            const float noise    = worldmap_noise[index]; 
            const float gradient = this->m_gradient[index];
            float value          = noise - gradient;

            if(value < 0.0f) value = 0.0f;
            if(value > 1.0f) value = 1.0f;

            if(value > this->settings.world_noise_terrain)
                this->m_region.regiontype.set_terrain();
        
            else
                this->m_region.regiontype.set_ocean();

            this->m_region.height            = std::ceil(value * 100) / 100;
            this->m_region.object_position.x = this->settings.world_panel_size.x * x; 
            this->m_region.object_position.y = this->settings.world_panel_size.y * y;
            this->m_region.object_size       = this->settings.world_panel_size;

            this->world_map[index] = this->m_region;
        }
    }

    // Find and mark coast tiles.
    // This loop marks tiles from 8 cardinal directions.
    for(int y = 0; y < this->settings.world_size; y++) {
        for(int x = 0; x < this->settings.world_size; x++) {
            const int index = this->wCalculateIndex(x, y);
            
            if(this->world_map[index].regiontype.is_terrain()) {
                const int index_up    = index - this->settings.world_size;             
                const int index_down  = index + this->settings.world_size;
                const int index_left  = index - 1;
                const int index_right = index + 1;
                const int index_tl    = index - 1 - this->settings.world_size;
                const int index_tr    = index + 1 - this->settings.world_size;
                const int index_bl    = index - 1 + this->settings.world_size;
                const int index_br    = index + 1 + this->settings.world_size;

                if(index_up > 0)
                    if(!this->world_map[index_up].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();

                if(index_left > 0)
                    if(!this->world_map[index_left].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();

                if(index_down < this->getWorldSize())
                    if(!this->world_map[index_down].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();

                if(index_right < this->getWorldSize())
                    if(!this->world_map[index_right].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();

                if(index_tl > 0)
                    if(!this->world_map[index_tl].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();

                if(index_tr > 0)
                    if(!this->world_map[index_tr].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();

                if(index_bl < this->getWorldSize())
                    if(!this->world_map[index_bl].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();

                if(index_br < this->getWorldSize())
                    if(!this->world_map[index_br].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();
            }
        }
    }

    // Generate climate.
    // This function calculates and assigns LATITUDE, TEMPERATURE and MOISTURE per region.
    this->worldmapGenerateClimate();

    // Assign biomes.
    // Biomes are assigned based on LATITUDE, TEMPERATURE and MOISTURE.
    // The distribution is similar to the one on earth.
    this->worldmapAssignBiome();

    // Make sure that there are no single full panel tiles.
    // This loop does not delete island tiles.
    for(int t = 0; t < 5; t++) {
        for(int y = 0; y < this->settings.world_size; y++) {
            for(int x = 0; x < this->settings.world_size; x++) {
                const int index = this->wCalculateIndex(x, y);
                auto& region = this->world_map[index];

                if(region.regiontype.is_terrain()) {
                    int terrain_tiles = 0;
    
                    if(index - 1 > 0)
                        if(this->world_map[index - 1].regiontype.is_terrain())
                            terrain_tiles++;
    
                    if(index + 1 < this->getWorldSize())
                        if(this->world_map[index + 1].regiontype.is_terrain())
                            terrain_tiles++; 
    
                    if(index - this->settings.world_size > 0)
                        if(this->world_map[index - this->settings.world_size].regiontype.is_terrain())
                            terrain_tiles++;
    
                    if(index + this->settings.world_size < this->getWorldSize())
                        if(this->world_map[index + this->settings.world_size].regiontype.is_terrain())
                            terrain_tiles++;
                        
                    if(terrain_tiles == 1) {
                        region.regiontype.unset_coast();
                        region.regiontype.set_ocean();;
                        region.biome = BIOME_OCEAN;
                        region.object_texture_name = this->createBiomeSpecificTexture("panel_full", BIOME_OCEAN); 
                    }
                }
            }
        }
    }

    // Assign pretty textures to coastal tiles.
    for(int y = 0; y < this->settings.world_size; y++) {
        for(int x = 0; x < this->settings.world_size; x++) {
            const int index = this->wCalculateIndex(x, y);
            auto& region    = this->world_map[index];

            region.object_texture_name = this->getWorldmapTile(index);
        }
    }

    std::cout << "[World Generation]: Generating world features.\n";

    this->worldmapGenerateRivers();
    
    // // Assign pretty textures to river tiles.
    // for(auto& pair : this->rivers) {
    //     const int index = pair.first;
    //     auto& river     = pair.second;

    //     river.object_texture_name = this->getRiverTile(index);
    // }

    this->worldmapGenerateForests();

    const float time_rounded = std::ceil(clock.getElapsedTime().asSeconds() * 100) / 100;
    std::cout << "[World Generation]: World generated in " << time_rounded << "s.\n";
}

void WorldGenerator::worldmapGenerateClimate() {
    std::cout << "[World Generation]: Generating climate.\n";

    // Calculate LATITUDE.
    for(int y = 0; y < this->settings.world_size; y++) {
        for(int x = 0; x < this->settings.world_size; x++) {       
            const int index = this->wCalculateIndex(x, y);

            // Latitude for the upper half.
            if(y < this->settings.world_size / 2)
                this->world_map[index].latitude = float(y + 1) / float(this->settings.world_size / 2);

            // Latitude for the bottom half.
            else {
                const int i = y - this->settings.world_size / 2;
                const int reversed_height = y - 2 * i;
                this->world_map[index].latitude = float(reversed_height) / float(this->settings.world_size / 2);
            }
        }
    }
    
    std::cout << "  [] Calculated latitude.\n";
    
    // Calculate TEMPERATURE.
    for(int y = 0; y < this->settings.world_size; y++) {
        for(int x = 0; x < this->settings.world_size; x++) {            
            const int panel_index = this->wCalculateIndex(x, y);
            Region& panel = this->world_map[panel_index];

            // Panel temperature zone classification.
            const bool PANEL_ARCTIC       = (y < this->settings.world_margin_poles || y > this->settings.world_size - this->settings.world_margin_poles - 1) ? true : false;
            const bool PANEL_NEAR_POLE    = ((y < this->settings.world_margin_poles + this->settings.world_margin_island + 0.1f * (float)this->settings.world_size && y >= this->settings.world_margin_poles) || (y >= this->settings.world_size - (this->settings.world_margin_poles + this->settings.world_margin_island + 0.1f * (float)this->settings.world_size) && y < this->settings.world_size - this->settings.world_margin_poles)) ? true : false;
            const bool PANEL_NEAR_EQUATOR = (y > this->settings.world_size / 2 - 1 - this->settings.world_margin_island && y <= this->settings.world_size / 2 + 1 + this->settings.world_margin_island) ? true : false;

            if(PANEL_ARCTIC) {
                panel.temperature = 0.05f + panel.latitude * panel.latitude + 1.0f / (float)(rand() % 10 + 5);
            }

            else if(PANEL_NEAR_POLE) {
                panel.temperature = 0.1f + panel.latitude * panel.latitude + 1.0f / (float)(rand() % 10 + 5);
            }
            
            else if(PANEL_NEAR_EQUATOR) {
                panel.temperature = -0.1f + panel.latitude * panel.latitude + 1.0f / (float)(rand() % 10 + 5);
            }

            else {
                panel.temperature = 0.4f + ((float)rand() / (float)RAND_MAX / 2) * ((1.0f - panel.height) * panel.latitude);
            }

            if(panel.temperature > 1.0f) panel.temperature = 1.0f;
        }
    }

    std::cout << "  [] Calculated temperature.\n";

    // Calculate MOISTURE.
    std::vector <float> input(this->settings.world_size * this->settings.world_size);
    for(int i = 0; i < this->settings.world_size * this->settings.world_size; i++) {
        input[i] = (float)rand() / (float)RAND_MAX;
    }
    
    float biggest_noise_recorded = 1.0f;

    for(int y = 0; y < this->settings.world_size; y++) {
        for(int x = 0; x < this->settings.world_size; x++) {
            const int index = this->wCalculateIndex(x, y);

            if(!this->world_map[index].regiontype.is_terrain())
                this->world_map[index].moisture = 1.0f;

            else if(this->is_arctic(index))
                this->world_map[index].moisture = 0.0f;

            else if(this->world_map[index].regiontype.is_terrain() && !this->is_arctic(index)) {
                float noise = 0.0f;
                float scale = 1.0f;
                float scale_acc = 0.0f;

                for(unsigned int o = 0; o < this->settings.world_moisture_octaves; o++) {
                    const int sampleX1 = (x / this->settings.world_moisture_persistence) * this->settings.world_moisture_persistence;
                    const int sampleY1 = (y / this->settings.world_moisture_persistence) * this->settings.world_moisture_persistence;

                    const int sampleX2 = (sampleX1 + this->settings.world_moisture_persistence) % (int)this->settings.world_size;					
                    const int sampleY2 = (sampleY1 + this->settings.world_moisture_persistence) % (int)this->settings.world_size;

                    const float blendX = (float)(x - sampleX1) / (float)this->settings.world_moisture_persistence;
                    const float blendY = (float)(y - sampleY1) / (float)this->settings.world_moisture_persistence;

                    const float sampleT = (1.0f - blendX) * input[sampleY1 * this->settings.world_size + sampleX1] + blendX * input[sampleY1 * this->settings.world_size + sampleX2];
                    const float sampleB = (1.0f - blendX) * input[sampleY2 * this->settings.world_size + sampleX1] + blendX * input[sampleY2 * this->settings.world_size + sampleX2];

                    scale_acc += scale;
                    noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
                    scale = float(scale / this->settings.world_moisture_bias);
                }
                
                float noise_value = (noise / scale_acc) * this->settings.world_moisture_multiplier;
                
                if(noise_value > biggest_noise_recorded) biggest_noise_recorded = noise_value;

                noise_value *= biggest_noise_recorded;
            
                if(this->world_map[index].height > 0.85f)
                    noise_value *= 0.1f + this->world_map[index].latitude * this->world_map[index].latitude * (1.0f - this->world_map[index].height);

                if(this->world_map[index].latitude < 0.55f) {
                    noise_value *= this->world_map[index].latitude * noise_value;
                    noise_value += 0.2f + (0.2f * noise_value);
                }

                else noise_value += 0.2f * this->world_map[index].moisture;

                if(noise_value > 1.0f) 
                    noise_value = 1.0f;

                this->world_map[y * this->settings.world_size + x].moisture = noise_value;
            }
        }
    }

    std::cout << "  [] Calculated moisture.\n";
}

void WorldGenerator::worldmapAssignBiome() {
    std::cout << "[World Generation]: Generating biomes.\n";
    
    for(int index = 0; index < this->getWorldSize(); index++) {
        Region& panel = this->world_map[index];

        if(!panel.regiontype.is_terrain())
            panel.biome = BIOME_OCEAN;
        
        else if(this->is_arctic(index))
            panel.biome = BIOME_ARCTIC;
    
        else if(panel.temperature < 0.45f && panel.latitude < 0.5f) {    
            if(panel.moisture > 0.23f)
                panel.biome = BIOME_CONTINENTAL;
            
            else
                panel.biome = BIOME_TUNDRA;
        }

        else if(panel.temperature < 0.55f && panel.latitude < 0.55f) {
            const int random_value = rand() % 10;

            if(random_value > 5)
                panel.biome = BIOME_TEMPERATE;

            else if(random_value > 2)
                panel.biome = BIOME_CONTINENTAL;
            
            else
                panel.biome = BIOME_TUNDRA;
        }

        else if(panel.temperature < 0.7f)
            panel.biome = BIOME_TEMPERATE;

        else {
            if(panel.moisture > 0.51f)
                panel.biome = BIOME_TROPICAL;

            else if(panel.moisture > 0.23f)
                panel.biome = BIOME_MEADITERRANEAN;

            else
                panel.biome = BIOME_TEMPERATE;
        } 
    }
}

void WorldGenerator::worldmapGenerateRivers() {
    std::cout << "  [] Generating rivers.\n";
    
    // Number of rivers is a maximum amount of rivers.
    // It does not mean that there will be the specified amount.

    // Storage for indexes of already created rivers.
    std::vector <int> river_origin_index(this->settings.world_river_quantity);
    const int area_around_origin = this->settings.world_river_scan_size;

    for(unsigned int river_number = 0; river_number < this->settings.world_river_quantity; river_number++) {
        int possible_river_origin_index = rand() % this->getWorldSize() - 1;
        bool place_found = false;

        while(!place_found) {
            int index = rand() % this->getWorldSize() - 1;
            
            if(this->world_map[index].regiontype.is_terrain() && !this->world_map[index].regiontype.is_coast() && !this->is_arctic(index) && !this->world_map[index].regiontype.is_forest()) {
                possible_river_origin_index = index;
                place_found = true;
            }
        }

        // Area (Panels) that should be scanned in each geographical direction from the possible river origin position. 
        unsigned int rivers_verified = 0;

        // Scan the provided area around the possible origin of the river to check if it is suitable.
        // The area is suitable if there are no rivers nearby.             
        for(unsigned int scanned_river = 0; scanned_river < river_origin_index.size(); scanned_river++) {
            int panels_verified = 0;
            int panels_occupied = 0;
            
            for(int y = 0 - area_around_origin; y <= 0 + area_around_origin; y++) {
                for(int x = 0 - area_around_origin; x <= 0 + area_around_origin; x++) {
                    const int index = possible_river_origin_index + this->wCalculateIndex(x, y);

                    if(index != river_origin_index[scanned_river] && !this->world_map[index].regiontype.is_river()) {
                        panels_verified++;
                        continue;
                    }

                    panels_occupied++;                
                }
            }    

            const int expected_verified_panels = std::pow(2 * area_around_origin + 1, 2); 
            if(panels_verified == expected_verified_panels && !(panels_occupied > 0)) {
                rivers_verified++;
            }
        }

        // Index at which a river will start. This index is verified.
        int river_index = -1;

        // If all rivers are verified for a possible river origin, then the place is valid and will be new river origin.
        if(rivers_verified == this->settings.world_river_quantity) {
            river_index = possible_river_origin_index;
            river_origin_index[river_number] = possible_river_origin_index;
        }

        else 
            continue;
        
        // Suitable position for a new river was not found; Ignore the rest of the algorithm and move on to the next river.
        if(river_index == -1)   
            continue;

        // Create a path to the sea.
        bool coast_found = false;
        int river_index_start   = river_index;
        int river_index_current = river_index;
        int last_move_value     = 0;

        // Create a path to the sea by following the lowest terrain until coast if found.
        while(true) {                
            this->world_map[river_index_current]._marked = true;

            const int index_up    = river_index_current - this->settings.world_size;
            const int index_down  = river_index_current + this->settings.world_size;
            const int index_left  = river_index_current - 1;
            const int index_right = river_index_current + 1;

            const int values[] = {
                index_up, 
                index_down, 
                index_left, 
                index_right
            };

            int current_move_direction = values[0];

            for(int i = 0; i < 4; i++) {
                if(!this->world_map[values[i]]._marked && this->world_map[current_move_direction].height > this->world_map[values[i]].height) {
                    current_move_direction = values[i];
                }
            }

            const int current_move_value = current_move_direction - river_index_current;

            RiverDirection direction      = RiverDirection::RIVER_ORIGIN;
            RiverDirection last_direction = direction;

            if(river_index_current != river_index_start) {
                if(current_move_direction == index_up) {
                    // UP AND RIGHT
                    if(current_move_value + last_move_value == -this->settings.world_size + 1) {
                        direction = RiverDirection::RIVER_NORTH_TO_WEST;
                    }

                    // UP AND LEFT
                    if(current_move_value + last_move_value == -this->settings.world_size - 1) {
                        direction = RiverDirection::RIVER_NORTH_TO_EAST;
                    }

                    // UP
                    if(current_move_value == -this->settings.world_size && last_move_value == -this->settings.world_size) {
                        direction = RiverDirection::RIVER_VERTICAL;
                    }

                }

                else if(current_move_direction == index_down) {
                    // DOWN AND RIGHT
                    if(current_move_value + last_move_value == this->settings.world_size + 1) {
                        direction = RiverDirection::RIVER_SOUTH_TO_WEST;
                    }

                    // DOWN AND LEFT
                    if(current_move_value + last_move_value == this->settings.world_size - 1) {
                        direction = RiverDirection::RIVER_SOUTH_TO_EAST;
                    }

                    // DOWN
                    if(current_move_value == this->settings.world_size && last_move_value == this->settings.world_size) {
                        direction = RiverDirection::RIVER_VERTICAL;
                    }
                }

                else if(current_move_direction == index_left) {
                    // LEFT
                    if(current_move_value + last_move_value == -1 || -2) {
                        direction = RiverDirection::RIVER_HORIZONTAL;
                    }

                    // LEFT AND UP
                    if(current_move_value + last_move_value == -1 - this->settings.world_size) {
                        direction = RiverDirection::RIVER_SOUTH_TO_WEST;
                    }

                    // LEFT AND DOWN
                    if(current_move_value + last_move_value == -1 + this->settings.world_size) {
                        direction = RiverDirection::RIVER_NORTH_TO_WEST;
                    }

                }

                else if(current_move_direction == index_right) {
                    // RIGHT
                    if(current_move_value + last_move_value == 1 || 2) {
                        direction = RiverDirection::RIVER_HORIZONTAL;
                    }

                    // RIGHT AND UP
                    if(current_move_value + last_move_value == 1 - this->settings.world_size) {
                        direction = RiverDirection::RIVER_SOUTH_TO_EAST;
                    }

                    // RIGHT AND DOWN
                    if(current_move_value + last_move_value == 1 + this->settings.world_size) {
                        direction = RiverDirection::RIVER_NORTH_TO_EAST;
                    }
                }

                else 
                    direction = RiverDirection::RIVER_NONE; 
            }

            Region& panel     = this->world_map[river_index_current];
            GameObject& river = this->rivers[river_index_current];
            const sf::Vector2f panel_position = panel.getPosition();
            const sf::Vector2f panel_offset   = sf::Vector2f(0, 0); 
            const sf::Vector2f panel_size     = this->settings.world_panel_size; 

            const std::string panel_full = this->createColouredTexture("panel_full", "panel_full_lightblue", COLOUR_CYAN, COLOUR_TRANSPARENT);
            switch(direction) {
                case RiverDirection::RIVER_NONE:
                    panel._direction = RiverDirection::RIVER_NONE;
                    break;

                case RiverDirection::RIVER_ORIGIN:
                    panel._direction = RiverDirection::RIVER_ORIGIN; 
                    break;
                
                case RiverDirection::RIVER_NORTH_TO_EAST:
                    panel._direction = RiverDirection::RIVER_NORTH_TO_EAST;
                    river = GameObject(panel_position, panel_offset, panel_size, this->getRiverTileVariation("panel_river_turn_bl")); 
                    break;

                case RiverDirection::RIVER_NORTH_TO_WEST:
                    panel._direction = RiverDirection::RIVER_NORTH_TO_WEST;
                    river = GameObject(panel_position, panel_offset, panel_size, this->getRiverTileVariation("panel_river_turn_br")); 
                    break;

                case RiverDirection::RIVER_SOUTH_TO_EAST:
                    panel._direction = RiverDirection::RIVER_SOUTH_TO_EAST;
                    river = GameObject(panel_position, panel_offset, panel_size, this->getRiverTileVariation("panel_river_turn_tl")); 
                    break;

                case RiverDirection::RIVER_SOUTH_TO_WEST:
                    panel._direction = RiverDirection::RIVER_SOUTH_TO_WEST;
                    river = GameObject(panel_position, panel_offset, panel_size, this->getRiverTileVariation("panel_river_turn_tr")); 
                    break;

                case RiverDirection::RIVER_VERTICAL:
                    panel._direction = RiverDirection::RIVER_VERTICAL;
                    river = GameObject(panel_position, panel_offset, panel_size, this->getRiverTileVariation("panel_river_vertical")); 
                    break;

                case RiverDirection::RIVER_HORIZONTAL:
                    panel._direction = RiverDirection::RIVER_HORIZONTAL;
                    river = GameObject(panel_position, panel_offset, panel_size, this->getRiverTileVariation("panel_river_horizontal")); 
                    break;
            }

            Region& start_panel = this->world_map[river_index_start];
            if(last_direction == RiverDirection::RIVER_ORIGIN) {
                if(river_index_current - 1 > 0)
                    if(this->world_map[river_index_current - 1].regiontype.is_river())
                        this->rivers[river_index_start] = GameObject(start_panel.getPosition(), sf::Vector2f(0, 0), start_panel.getSize(), this->getRiverTileVariation("panel_river_origin_right"));

                else if(river_index_current - this->settings.world_size > 0)
                    if(this->world_map[river_index_current - this->settings.world_size].regiontype.is_river())
                        this->rivers[river_index_start] = GameObject(start_panel.getPosition(), sf::Vector2f(0, 0), start_panel.getSize(), this->getRiverTileVariation("panel_river_origin_down"));
                
                else if(river_index_current + 1 < this->getWorldSize())
                    if(this->world_map[river_index_current + 1].regiontype.is_river())
                        this->rivers[river_index_start] = GameObject(start_panel.getPosition(), sf::Vector2f(0, 0), start_panel.getSize(), this->getRiverTileVariation("panel_river_origin_left"));
                
                else if(river_index_current + this->settings.world_size < this->getWorldSize())
                    if(this->world_map[river_index_current + this->settings.world_size].regiontype.is_river())
                        this->rivers[river_index_start] = GameObject(start_panel.getPosition(), sf::Vector2f(0, 0), start_panel.getSize(), this->getRiverTileVariation("panel_river_origin_up"));
            }

            panel.regiontype.set_river();
            last_move_value     = current_move_value;
            river_index_current = current_move_direction;
            last_direction      = direction;

            if(coast_found)
                break;

            if(this->world_map[river_index_current].regiontype.is_coast())
                coast_found = true;
        }
    }
}

void WorldGenerator::worldmapGenerateForests() {
    std::cout << "  [] Generating forests.\n";

    const int WORLD_SIZE = this->getWorldSize();
    NoiseContainer noise_forest(WORLD_SIZE);

    NoiseSettings settings;
    settings.octaves     = 8;
    settings.bias        = 4;
    settings.persistence = 4;
    settings.multiplier  = 1.25f;
    settings.size        = sf::Vector2f(this->settings.world_size, this->settings.world_size);

    this->generateNoise(settings, noise_forest);

    for(int index = 0; index < noise_forest.size(); index++) {
        const float noise  = noise_forest[index];
        const float random = (float)rand() / (float)RAND_MAX;
        const float value  = (noise + random) / 2;

        Region& region = this->world_map[index];

        const bool is_ocean   = this->is_ocean(index);
        const bool is_arctic  = this->is_arctic(index);
        const bool is_desert  = this->is_desert(index);
        const bool is_river   = region.regiontype.is_river();
        const bool is_coast   = region.regiontype.is_coast();

        // If noise is high enough and tile is valid, place a forest. 
        if(value > 0.7f && !is_arctic && !is_desert && !is_ocean && !is_coast && !is_river) {
            region.regiontype.set_forest();            
            const std::string texture_name = this->getWorldmapTreeTextureName(region.biome);
            this->forests[index] = GameObject(region.getPosition(), sf::Vector2f(0, 0), region.getSize(), texture_name);
        }
    }
}

void WorldGenerator::generateRegion(int index, Region& region) {
    sf::Clock clock;

    std::cout << "[World Generation][Region Generation]: Beginning to generate region " << index << ".\n"; 
    
    // Do this because if at least one region was visitied,
    // the m_tile property still holds value from the last region generation.
    this->m_tile = Tile();

    this->m_tile.object_size         = this->settings.region_tile_size;
    this->m_tile.object_texture_name = this->getBiomeTileTextureName(region.biome);

    // Create a flat map populated with tiles on the first elevation layer (0-th).
    region.map.resize(this->getRegionSize());
    for(int y = 0; y < this->settings.region_size; y++) {
        for(int x = 0; x < this->settings.region_size; x++) {
            const int index              = this->rCalculateIndex(x, y);
            this->m_tile.object_position = this->tilePositionScreen(x, y);
            region.map[index]            = this->m_tile;
        }
    }
    
    const bool region_terrain = region.regiontype.is_terrain();
    if(region_terrain) {
        this->regionGenerateRiver (index);   
        this->regionGenerateHeight(index);
        this->regionGenerateForest(index);
    }

    // Make sure that if the river exists, the tiles that are water are placed on the lowest elevation.
    if(region_terrain && region.regiontype.is_river()) {
        if(region._direction != RiverDirection::RIVER_NONE) {
            for(int y = 0; y < this->settings.region_size; y++) {
                for(int x = 0; x < this->settings.region_size; x++) {
                    const int index = this->rCalculateIndex(x, y);
    
                    if(region.map[index].tiletype.is_river()) {
                        region.map[index].elevation = 0;
                        region.map[index].object_position = this->tilePositionScreen(x, y);
                    }
                }
            }
        }
    }

    // Make sure that high tiles have sides.
    for(int y = 0; y < this->settings.region_size; y++) {
        for(int x = 0; x < this->settings.region_size; x++) {
            const int index = this->rCalculateIndex(x, y);

            // Indexes to check.
            const int index_bottom = index + this->settings.region_size;
            const int index_right  = index + 1;
            const int index_left   = index - 1;
            
            // To avoid blank spots, you only fill in the missing height, when the biggest slope was found.
            int biggest_difference = 0;

            if(index_bottom < this->getRegionSize()) {
                if(region.map[index].elevation > region.map[index_bottom].elevation) {
                    const int height_difference = std::abs(region.map[index].elevation - region.map[index_bottom].elevation);
                    if(biggest_difference < height_difference)
                        biggest_difference = height_difference;
                }
            }

            if(index_right < this->getRegionSize()) {
                if(region.map[index].elevation > region.map[index_right].elevation) {
                    const int height_difference = std::abs(region.map[index].elevation - region.map[index_right].elevation);
                    if(biggest_difference < height_difference)
                        biggest_difference = height_difference;
                }
            }

            if(index_left > 0) {
                if(region.map[index].elevation > region.map[index_left].elevation) {
                    const int height_difference = std::abs(region.map[index].elevation - region.map[index_left].elevation);
                    if(biggest_difference < height_difference)
                        biggest_difference = height_difference;
                }
            }

            region.sides[index].resize(biggest_difference);
            for(int i = 0; i < biggest_difference; i++) {
                std::string tile = i > 2 ? "tile_height_stone" : "tile_height_dirt";
                region.sides[index][i] = GameObject(region.map[index].getPosition(), sf::Vector2f(0, this->settings.region_tile_size.y / 2 + i * this->settings.region_tile_size.y / 2), this->settings.region_tile_size, tile);
            }

            // Make sure that the border tiles have depth.
            // Tiles near the y edge.
            if(y == this->settings.region_size - 1) {
                const int height_difference = std::abs(0 - region.map[(this->settings.region_size - 1) * this->settings.region_size + x].elevation) + 1;
                region.sides[index].resize(height_difference);
                
                for(int i = 0; i < height_difference; i++) {
                    std::string tile = i > 2 ? "tile_height_stone" : "tile_height_dirt";
                    region.sides[index][i] = GameObject(region.map[index].getPosition(), sf::Vector2f(0, this->settings.region_tile_size.y / 2 + i * this->settings.region_tile_size.y / 2), this->settings.region_tile_size, tile);
                }
            }

            // Tiles near the x edge.
            if(x == this->settings.region_size - 1) {
                const int height_difference = std::abs(0 - region.map[y * this->settings.region_size + this->settings.region_size - 1].elevation) + 1;
                region.sides[index].resize(height_difference);
                
                for(int i = 0; i < height_difference; i++) {
                    std::string tile = i > 2 ? "tile_height_stone" : "tile_height_dirt";
                    region.sides[index][i] = GameObject(region.map[index].getPosition(), sf::Vector2f(0, this->settings.region_tile_size.y / 2 + i * this->settings.region_tile_size.y / 2), this->settings.region_tile_size, tile);
                }
            }
        }
    }

    if(rand() % this->getWorldSize() < 0.4f * this->getWorldSize()) {
        NoiseSettings settings = NoiseSettings(sf::Vector2f(this->settings.region_size, this->settings.region_size), 16, 16, 4, 1.00f);
        std::vector <float> noise_stone;
        this->generateNoise(settings, noise_stone);
    
        for(int index = 0; index < noise_stone.size(); index++) { 
            if(noise_stone[index] > 0.85f && !region.trees.count(index) && !region.map[index].tiletype.is_river()) {
                region.map[index].object_texture_name = "tile_resource_stone";
            }
        }
    }

    region.visited = true;

    std::cout.precision(3);
    std::cout << "[World Generation][Region Generation]: Region " << index << " generated in " << clock.getElapsedTime().asSeconds() << " seconds.\n";
    std::cout.precision(6);
}

void WorldGenerator::generateNoise(NoiseSettings& settings, std::vector<float>& storage) {
    // Generate a random input sequence.
    std::vector <float> input(settings.size.x * settings.size.y);
    for(int i = 0; i < input.size(); i++) {
        input[i] = (float)rand() / (float)RAND_MAX;
    }

    storage.resize(settings.size.x * settings.size.y);

    float amplifier = 1.0f;

    const auto assertIndexSize = [](int& index_value, int limit) -> void {
        if(index_value > limit) {
            index_value = limit - 1;
        }
    };

    // Generate the noise.
    for(int y = 0; y < settings.size.y; y++) {
        for(int x = 0; x < settings.size.x; x++) {
            float noise = 0.0f;       // Noise accumulator.
            float scale = 1.0f;       // The frequency of the noise.
            float scale_check = 0.0f; // Scale check to keep the noise values between 0 and 1.

            for(int o = 0; o < settings.octaves; o++) {
                const int sampleX1 = (x / settings.persistence) * settings.persistence;
                const int sampleY1 = (y / settings.persistence) * settings.persistence;
                
                const int sampleX2 = (sampleX1 + settings.persistence) % (int)settings.size.x;					
                const int sampleY2 = (sampleY1 + settings.persistence) % (int)settings.size.x;
                
                const float blendX = (float)(x - sampleX1) / (float)settings.persistence;
                const float blendY = (float)(y - sampleY1) / (float)settings.persistence;
                
                int index1 = sampleY1 * (int)settings.size.x + sampleX1;
                int index2 = sampleY2 * (int)settings.size.x + sampleX1;
                int index3 = sampleY1 * (int)settings.size.x + sampleX2;
                int index4 = sampleY2 * (int)settings.size.x + sampleX2;

                assertIndexSize(index1, input.size());
                assertIndexSize(index2, input.size());
                assertIndexSize(index3, input.size());
                assertIndexSize(index4, input.size());

                const float sampleT = (1.0f - blendX) * input.at(index1) + blendX * input.at(index3);
                const float sampleB = (1.0f - blendX) * input.at(index2) + blendX * input.at(index4);
                            
                scale_check += scale;
                noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
                scale = float(scale / settings.bias);
            }

            // Do not change this.
            // World size and region size may be two different values.
            int index = y * settings.size.x + x;

            float buffer_noise = (noise / scale_check) * settings.multiplier;
            
            if(buffer_noise > amplifier) amplifier = buffer_noise;
            buffer_noise *= amplifier;

            storage[index] = (buffer_noise > 1.0f) ? 1.0f : buffer_noise; 
        }
    }
}

std::string WorldGenerator::getBiomeTileTextureName(Biome biome) {
    if(biome.biome_name == "Continental")
        return "tile_grass_cold";

    else if(biome.biome_name == "Temperate")    
        return "tile_grass_warm";
        
    else if(biome.biome_name == "Tropical")
        return "tile_grass_tropical";

    else if(biome.biome_name == "Mediterranean")
        return "tile_grass_subtropical";

    else if(biome.biome_name == "Ocean")
        return "tile_ocean";

    else if(biome.biome_name == "Sea")
        return "tile_sea";

    else if(biome.biome_name == "Tundra")
        return "tile_tundra";

    else if(biome.biome_name == "Arctic")
        return "tile_arctic";

    else if(biome.biome_name == "Desert")
        return "tile_desert";

    else return "default";
}

sf::Vector2f WorldGenerator::tilePositionScreen(int x, int y) {
    return sf::Vector2f(
        (this->settings.region_tile_offset.x * this->settings.region_tile_size.x) + (x - y) * (this->settings.region_tile_size.x / 2),
        (this->settings.region_tile_offset.y * this->settings.region_tile_size.y) + (x + y) * (this->settings.region_tile_size.y / 2)
    );
}

sf::Vector2f WorldGenerator::tilePositionScreen(sf::Vector2i tile_position) {
    return this->tilePositionScreen(tile_position.x, tile_position.y);
}

sf::Vector2f WorldGenerator::tilePositionScreen(sf::Vector2f tile_position) {
    return this->tilePositionScreen(sf::Vector2i(tile_position.x, tile_position.y));
}

bool WorldGenerator::is_biome(int region_index, Biome biome) const {
    return this->world_map[region_index].biome.biome_name == biome.biome_name;
}

bool WorldGenerator::is_arctic(int region_index) const {
    return this->is_biome(region_index, BIOME_ARCTIC);
}

bool WorldGenerator::is_ocean(int region_index) const {
    return this->is_biome(region_index, BIOME_OCEAN);
}

bool WorldGenerator::is_sea(int region_index) const {
    return this->is_biome(region_index, BIOME_SEA);
}

bool WorldGenerator::is_tropical(int region_index) const {
    return this->is_biome(region_index, BIOME_TROPICAL);
}

bool WorldGenerator::is_mediterranean(int region_index) const {
    return this->is_biome(region_index, BIOME_MEADITERRANEAN);
}

bool WorldGenerator::is_temperate(int region_index) const {
    return this->is_biome(region_index, BIOME_TEMPERATE);
}

bool WorldGenerator::is_continental(int region_index) const {
    return this->is_biome(region_index, BIOME_CONTINENTAL);
}

bool WorldGenerator::is_tundra(int region_index) const {
    return this->is_biome(region_index, BIOME_TUNDRA);
}

bool WorldGenerator::is_desert(int region_index) const {
    return this->is_biome(region_index, BIOME_DESERT);
}

bool WorldGenerator::is_terrain(int region_index) const {
    return region_index > this->settings.world_noise_terrain;
}

bool WorldGenerator::is_coast(int region_index) const {
    return this->world_map.at(region_index).regiontype.is_coast();
}

std::string WorldGenerator::getTreeTextureNameRegion(Biome biome) {
    if(biome.biome_name == "Temperate") {
        int value = std::rand() % 2;

        return (value > 0) 
        ? "tile_tree_warm1"
        : "tile_tree_warm2";
    }

    else if(biome.biome_name == "Continental" || biome.biome_name == "Tundra")
        return "tile_tree_cold1";

    else if(biome.biome_name == "Tropical" || biome.biome_name == "Mediterranean")
        return "tile_tree_tropical1";

    else return "default";
}

std::string WorldGenerator::getTilePixelColour(sf::Vector2i pixel) {
    if(pixel.x < 0 || pixel.y < 0) 
        return "Other";

    if(pixel.x > this->settings.region_tile_size.x - 1 || pixel.y > this->settings.region_tile_size.y - 1) 
        return "Other";

    auto pixel_colour = this->resource->getTexture("tile_template_direction").copyToImage().getPixel(pixel.x, pixel.y);

    if(pixel_colour == sf::Color::Red)         return "Red";
    else if(pixel_colour == sf::Color::Green)  return "Green";
    else if(pixel_colour == sf::Color::Blue)   return "Blue";
    else if(pixel_colour == sf::Color::Yellow) return "Yellow";
    else return "Other";
}

// Returns the index of a tile under mouse pointer.
// If index is not found, returns -1.
int WorldGenerator::getTileIndex(sf::Vector2f mouse_position, Region& region) {
    // Tiles which intersect with the mouse position.
    std::vector <int> tiles;
    
    int last_index = -1;
    for(int x = 0; x < this->settings.region_size; x++) {
        for(int y = 0; y < this->settings.region_size; y++) {
            const int index  = this->rCalculateIndex(x, y);
            const Tile& tile = region.map[index];
            sf::FloatRect tile_rectangle  (tile.getPosition(), tile.getSize());
            sf::FloatRect cursor_rectangle(mouse_position, sf::Vector2f(8, 8));

            if(tile_rectangle.contains(mouse_position) && tile_rectangle.intersects(cursor_rectangle)) {
                tiles.push_back(index);
                last_index = index;
            }
        }
    }   

    if(tiles.size() != 2)
        return last_index;

    auto& tile1 = region.map[tiles[0]]; 
    auto& tile2 = region.map[tiles[1]];

    auto pos1 = sf::Vector2f(
        std::abs(mouse_position.x - tile1.getPosition().x + tile1.getSize().x), 
        std::abs(mouse_position.y - tile1.getPosition().y + tile1.getSize().y) 
    );    

    auto pos2 = sf::Vector2f(
        std::abs(mouse_position.x - tile2.getPosition().x + tile2.getSize().x / 2),
        std::abs(mouse_position.y - tile2.getPosition().y + tile2.getSize().y / 2)
    );

    // If the cursor is closer to the center of one tile.
    if((pos1.x < pos2.x && pos1.y < pos2.y))
        return tiles[0];
    
    else
        return tiles[1];
}

void WorldGenerator::regionGenerateRiver(int region_index) {    
    Region& region = this->world_map[region_index];

    if(!region.regiontype.is_river())
        return;

    std::cout << "[World Generation][Region Generation]: Generating river.\n";

    const int river_size = this->settings.region_size / 10 / 2;
    RiverDirection direction = region.riverDirection();

    switch(direction) {
        default: {
            std::cout << "[Error][Region Generation]: Unsupported river direction.\n";
            break;
        }
        
        case RiverDirection::RIVER_NONE: {
            std::cout << "[World Generation][Region Generation]: River does not have a adjacent river.\n";
            break;
        }
        
        case RiverDirection::RIVER_ORIGIN: {
            char adjacent_region_direction = '!';

            if(this->world_map[region_index + 1].regiontype.is_river()) {
                adjacent_region_direction = 'E';
            }

            if(this->world_map[region_index - 1].regiontype.is_river()) {
                adjacent_region_direction = 'W';
            }

            if(this->world_map[region_index + this->settings.region_size].regiontype.is_river()) {
                adjacent_region_direction = 'S';
            }

            if(this->world_map[region_index - this->settings.region_size].regiontype.is_river()) {
                adjacent_region_direction = 'N';
            }

            if(adjacent_region_direction == '!') {
                std::cout << "[Error][Region Generation]: There is no adjacent region to river origin.\n";
                return;
            }

            std::cout << "[World Generation][Region Generation]: River direction is '" << adjacent_region_direction << "'.\n"; 

            const auto pond_radius = (this->settings.region_size + this->settings.region_size) / 2 / 10;
            const auto pond_centre = sf::Vector2f(this->settings.region_size / 2, this->settings.region_size / 2);

            // Draw the pond itself.
            for(int x = 0; x < this->settings.region_size; x++) {
                for(int y = 0; y < this->settings.region_size; y++) {
                    const int  index = this->rCalculateIndex(x, y);
                    const bool in_circle = (x - pond_centre.x) * (x - pond_centre.x) + (y - pond_centre.y) * (y - pond_centre.y) < pond_radius * pond_radius; 

                    if(in_circle) {
                        region.map[index].object_texture_name = "tile_sea";
                        region.map[index].tiletype.set_river();
                    }
                }
            }

            // Draw the river "connecting" the pond with adjacent regions.
            switch(adjacent_region_direction) {
                default: {
                    std::cout << "[Error][Region Generator]: Could not generate river connection.\n";
                    break;                 
                }

                case 'N': {
                    NoiseSettings settings;
                    settings.size        = sf::Vector2f(this->settings.region_size / 2, 1);
                    settings.bias        = 6;
                    settings.octaves     = 10;
                    settings.persistence = 12;
                    settings.multiplier  = 1.25;

                    std::vector <float> disortion;
                    this->generateNoise(settings, disortion);

                    for(int x = this->settings.region_size / 2 - river_size; x < this->settings.region_size / 2 + river_size; x++) {
                        for(int y = 0; y < this->settings.region_size / 2; y++) {
                            const int value = disortion[y] / 0.15f;
                            const int index = (x + value) * this->settings.region_size + y;

                            region.map[index].object_texture_name = "tile_sea";
                            region.map[index].tiletype.set_river();
                        }
                    }

                    break;
                }
                
                case 'S': {
                    NoiseSettings settings;
                    settings.size        = sf::Vector2f(this->settings.region_size / 2, 1);
                    settings.bias        = 6;
                    settings.octaves     = 10;
                    settings.persistence = 12;
                    settings.multiplier  = 1.25;

                    std::vector <float> disortion;
                    this->generateNoise(settings, disortion);

                    for(int x = this->settings.region_size / 2 - river_size; x < this->settings.region_size / 2 + river_size; x++) {
                        for(int y = this->settings.region_size / 2; y < this->settings.region_size; y++) {
                            const int value = disortion[y - this->settings.region_size / 2] / 0.15f;
                            const int index = (x + value) * this->settings.region_size + y;

                            region.map[index].object_texture_name = "tile_sea";;
                            region.map[index].tiletype.set_river();                        
                        }
                    }

                    break;
                }

                case 'W': {
                    NoiseSettings settings;
                    settings.size        = sf::Vector2f(this->settings.region_size / 2, 1);
                    settings.bias        = 6;
                    settings.octaves     = 10;
                    settings.persistence = 12;
                    settings.multiplier  = 1.25;
        
                    std::vector <float> disortion;
                    this->generateNoise(settings, disortion);
        
                    for(int x = this->settings.region_size / 2; x < this->settings.region_size; x++) {
                        for(int y = this->settings.region_size / 2 - river_size; y < this->settings.region_size / 2 + river_size; y++) {
                            const int value = disortion[x - this->settings.region_size / 2] / 0.15f;
                            const int index = x * this->settings.region_size + y + value;
                            
                            region.map[index].object_texture_name = "tile_sea";
                            region.map[index].tiletype.set_river();
                        }
                    }
                    
                    break;
                }

                case 'E': {
                    NoiseSettings settings;
                    settings.size        = sf::Vector2f(this->settings.region_size / 2, 1);
                    settings.bias        = 6;
                    settings.octaves     = 10;
                    settings.persistence = 12;
                    settings.multiplier  = 1.25;
        
                    std::vector <float> disortion;
                    this->generateNoise(settings, disortion);
        
                    for(int x = 0; x < this->settings.region_size / 2; x++) {
                        for(int y = this->settings.region_size / 2 - river_size; y < this->settings.region_size / 2 + river_size; y++) {
                            const int value = disortion[x] / 0.15f;
                            const int index = x * this->settings.region_size + y + value;
                            
                            region.map[index].object_texture_name = "tile_sea";
                            region.map[index].tiletype.set_river();
                        }
                    }

                    break;
                }
            }

            break;
        }
    
        case RiverDirection::RIVER_HORIZONTAL: {        
            NoiseSettings settings;
            settings.size        = sf::Vector2f(this->settings.region_size, 1);
            settings.bias        = 6;
            settings.octaves     = 10;
            settings.persistence = 12;
            settings.multiplier  = 1.25;

            std::vector <float> disortion(this->settings.region_size);
            this->generateNoise(settings, disortion);

            for(int x = 0; x < this->settings.region_size; x++) {
                for(int y = this->settings.region_size / 2 - river_size; y < this->settings.region_size / 2 + river_size; y++) {
                    const int value = disortion[x] / 0.15f;
                    const int index = x * this->settings.region_size + y + value;
                    
                    region.map[index].object_texture_name = "tile_sea";
                    region.map[index].tiletype.set_river();
                }
            }

            break;
        }

        case RiverDirection::RIVER_VERTICAL: {
            NoiseSettings settings;
            settings.size        = sf::Vector2f(this->settings.region_size, 1);
            settings.bias        = 6;
            settings.octaves     = 10;
            settings.persistence = 12;
            settings.multiplier  = 1.25;

            std::vector <float> disortion(this->settings.region_size);
            this->generateNoise(settings, disortion);

            for(int x = this->settings.region_size / 2 - river_size; x < this->settings.region_size / 2 + river_size; x++) {
                for(int y = 0; y < this->settings.region_size; y++) {
                    const int value = disortion[y] / 0.15f;
                    const int index = (x + value) * this->settings.region_size + y;

                    region.map[index].object_texture_name = "tile_sea";
                    region.map[index].tiletype.set_river();
                }
            }

            break;
        }

        case RiverDirection::RIVER_NORTH_TO_EAST: {            
            const sf::Vector2i relative_point = sf::Vector2i(0, 0);
            for(int x = 0; x < this->settings.region_size / 2; x++) {
                for(int y = 0; y < this->settings.region_size / 2; y++) {
                    const int   index = this->rCalculateIndex(x, y);
                    const float distance_x = (relative_point.x - x) * (relative_point.x - x);
                    const float distance_y = (relative_point.y - y) * (relative_point.y - y);
                    const float distance = sqrt(distance_x + distance_y) / (this->settings.region_size / 4 * 2);

                    if(distance > 0.85f && distance < 1.05f) {
                        region.map[index].object_texture_name = "tile_sea";
                        region.map[index].tiletype.set_river();
                    }
                }
            }

            break;
        }

        case RiverDirection::RIVER_NORTH_TO_WEST: {
            const sf::Vector2i relative_point = sf::Vector2i(0, this->settings.region_size);
            for(int x = 0; x < this->settings.region_size / 2; x++) {
                for(int y = this->settings.region_size / 2; y < this->settings.region_size; y++) {
                    const int   index = this->rCalculateIndex(x, y);
                    const float distance_x = (relative_point.x - x) * (relative_point.x - x);
                    const float distance_y = (relative_point.y - y) * (relative_point.y - y);
                    const float distance = sqrt(distance_x + distance_y) / (this->settings.region_size / 4 * 2);

                    if(distance > 0.85f && distance < 1.05f) {
                        region.map[index].object_texture_name = "tile_sea";
                        region.map[index].tiletype.set_river();
                    }
                }
            }

            break;
        }

        case RiverDirection::RIVER_SOUTH_TO_EAST: {
            const sf::Vector2i relative_point = sf::Vector2i(this->settings.region_size, 0);
            for(int x = this->settings.region_size / 2; x < this->settings.region_size; x++) {
                for(int y = this->settings.region_size / 2; y >= 0; y--) {
                    const int   index = this->rCalculateIndex(x, y);
                    const float distance_x = (relative_point.x - x) * (relative_point.x - x);
                    const float distance_y = (relative_point.y - y) * (relative_point.y - y);
                    const float distance = sqrt(distance_x + distance_y) / (this->settings.region_size / 4 * 2);

                    if(distance > 0.85f && distance < 1.05f) {
                        region.map[index].object_texture_name = "tile_sea";
                        region.map[index].tiletype.set_river();
                    }
                }
            }
            
            break;
        }

        case RiverDirection::RIVER_SOUTH_TO_WEST: {
            const sf::Vector2i relative_point = sf::Vector2i(this->settings.region_size, this->settings.region_size);
            for(int x = this->settings.region_size - 1; x >= this->settings.region_size / 2; x--) {
                for(int y = this->settings.region_size - 1; y >= this->settings.region_size / 2; y--) {
                    const int   index = this->rCalculateIndex(x, y);
                    const float distance_x = (relative_point.x - x) * (relative_point.x - x);
                    const float distance_y = (relative_point.y - y) * (relative_point.y - y);
                    const float distance = sqrt(distance_x + distance_y) / (this->settings.region_size / 4 * 2);

                    if(distance > 0.85f && distance < 1.05f) {
                        region.map[index].object_texture_name = "tile_sea";
                        region.map[index].tiletype.set_river();
                    }
                }
            }

            break;
        }
    }
}

void WorldGenerator::regionGenerateForest(int region_index) {
    Region& region = this->world_map[region_index];

    NoiseSettings settings;

    const bool dense = region.regiontype.is_forest();

    if(dense)
        settings = NoiseSettings(sf::Vector2f(this->settings.region_size, this->settings.region_size), 8, 12, 4, 1.25f);
    
    else
        settings = NoiseSettings(sf::Vector2f(this->settings.region_size, this->settings.region_size), 4, 8, 4, 0.90f);
    
    std::string forest_type = dense ? "dense" : "sparse";
    std::cout << "[World Generation][Region Generation]: Generating a " << forest_type << " forest.\n";

    std::vector <float> noise;
    this->generateNoise(settings, noise);

    for(int i = 0; i < noise.size(); i++) {
        auto& tile = region.map[i];
        if(!tile.tiletype.is_river() && noise[i] > 0.7f) {
            std::pair <int, GameObject> pair(i, GameObject(tile.getPosition(), sf::Vector2f(0, 1.5f * -this->settings.region_tile_size.y), sf::Vector2f(this->settings.region_tile_size.x, 2 * this->settings.region_tile_size.y), this->getTreeTextureNameRegion(region.biome)));
            region.trees.insert(pair);
        }
    }
}

void WorldGenerator::regionGenerateHeight(int region_index) {
    Region& region = this->world_map[region_index];

    std::cout << "[World Generation][Region Generation]: Generating flatlands.\n";

    // Height modifier for fancier terrain.
    const float height_modifier = 1.2f;

    NoiseSettings height_settings(sf::Vector2f(this->settings.region_size, this->settings.region_size), 16, 16, 4, height_modifier);
    std::vector <float> height;
    this->generateNoise(height_settings, height);

    const RiverDirection river_direction = region._direction;

    for(int i = 0; i < region.map.size(); i++) {
        region.map[i]._marked = false;
    }

    // How many tiles from the river should be smoothed to create a valley-like look.
    const int terrain_slope = this->settings.region_size / 10;

    const auto smaller_slope = [&](int slope) -> float {
        return std::floor(std::abs(std::sqrt(slope) - 1));
    };

    for(int y = 0; y < this->settings.region_size; y++) {
        for(int x = 0; x < this->settings.region_size; x++) {
            const int index = this->rCalculateIndex(x, y);

            // For tiles that follow a river the elevation should be similar to a valley.
            if(region.map[index].tiletype.is_river()) {
                if(river_direction == RiverDirection::RIVER_HORIZONTAL) {
                    // For the southern coast.
                    if(index - 1 >= 0) {
                        if(!region.map[index - 1].tiletype.is_river()) {
                            for(int slope_left = 0; slope_left < terrain_slope; slope_left++) {
                                const int index_left = index - slope_left;
                    
                                if(index_left >= 0) {
                                    if(!region.map[index_left].tiletype.is_river() && !region.map[index_left]._marked) {
                                        const int elevation = smaller_slope(slope_left);  
                                        region.map[index_left]._marked = true;
                                        region.map[index_left].elevation = elevation;
                                        region.map[index_left].tiletype.set_terrain();
                                    }
                                }
                            }
                        }
                    }

                    // For the northern coast.
                    if(index + 1 < this->getRegionSize()) {
                        if(!region.map[index + 1].tiletype.is_river()) {
                            for(int slope_right = 0; slope_right < terrain_slope; slope_right++) {
                                const int index_right = index + slope_right;
    
                                if(index_right < this->getRegionSize()) {
                                    if(!region.map[index_right].tiletype.is_river() && !region.map[index_right]._marked) {
                                        const int elevation = smaller_slope(slope_right); 
                                        region.map[index_right]._marked = true;
                                        region.map[index_right].elevation = elevation;
                                        region.map[index_right].tiletype.set_terrain();
                                    }
                                }
                            }
                        }
                    }
                }                      

                if(river_direction == RiverDirection::RIVER_VERTICAL) {
                    // For the western coast.
                    if(index - this->settings.region_size >= 0) {
                        if(!region.map[index - this->settings.region_size].tiletype.is_river()) {
                            for(int slope_top = 0; slope_top < terrain_slope; slope_top++) {
                                const int index_top = index - slope_top * this->settings.region_size;

                                if(index_top >= 0) {
                                    if(!region.map[index_top].tiletype.is_river() && !region.map[index_top]._marked) {
                                        const int elevation = smaller_slope(slope_top); 
                                        region.map[index_top]._marked = true;
                                        region.map[index_top].elevation = elevation;
                                        region.map[index_top].tiletype.set_terrain();
                                    }
                                }
                            }
                        }
                    } 

                    // For the eastern coast.
                    if(index + this->settings.region_size < this->getRegionSize()) {
                        if(!region.map[index + this->settings.region_size].tiletype.is_river()) {
                            for(int slope_bottom = 0; slope_bottom < terrain_slope; slope_bottom++) {
                                const int index_bottom = index + slope_bottom * this->settings.region_size;

                                if(index_bottom < this->getRegionSize()) {
                                    if(!region.map[index_bottom].tiletype.is_river() && !region.map[index_bottom]._marked) {
                                        const int elevation = smaller_slope(slope_bottom); 
                                        region.map[index_bottom]._marked = true;
                                        region.map[index_bottom].elevation = elevation;
                                        region.map[index_bottom].tiletype.set_terrain();
                                    }
                                }                                
                            }
                        }
                    }
                } 
            
                if(river_direction == RiverDirection::RIVER_NORTH_TO_EAST || river_direction == RiverDirection::RIVER_NORTH_TO_WEST || river_direction == RiverDirection::RIVER_SOUTH_TO_EAST || river_direction == RiverDirection::RIVER_SOUTH_TO_WEST || river_direction == RiverDirection::RIVER_ORIGIN) {
                    // For the western coast.
                    if(index - 1 >= 0 && index - 1 >= y * this->settings.region_size) {
                        if(!region.map[index - 1].tiletype.is_river()) {
                            for(int slope_left = 0; slope_left < terrain_slope; slope_left++) {
                                const int index_left = index - slope_left;

                                if(index_left >= 0) {
                                    if(!region.map[index_left].tiletype.is_river() && !region.map[index_left]._marked) {
                                        const int elevation = smaller_slope(slope_left);
                                        region.map[index_left]._marked = true;
                                        region.map[index_left].elevation = elevation;
                                        region.map[index_left].tiletype.set_terrain();
                                    }
                                }
                            }
                        }
                    }

                    // For the eastern coast.
                    if(index + 1 < this->getRegionSize() && index + 1 < (y + 1) * this->settings.region_size) {
                        if(!region.map[index + 1].tiletype.is_river()) {
                            for(int slope_right = 0; slope_right < terrain_slope; slope_right++) {
                                const int index_right = index + slope_right;

                                if(index_right < this->getRegionSize()) {
                                    if(!region.map[index_right].tiletype.is_river() && !region.map[index_right]._marked) {
                                        const int elevation = smaller_slope(slope_right);
                                        region.map[index_right]._marked = true;
                                        region.map[index_right].elevation = elevation;
                                        region.map[index_right].tiletype.set_terrain();
                                    }
                                }
                            }
                        }
                    }

                    // For the northern coast.
                    if(index - this->settings.region_size >= 0) {
                        if(!region.map[index - this->settings.region_size].tiletype.is_river()) {
                            for(int slope_left = 0; slope_left < terrain_slope; slope_left++) {
                                const int index_left = index - slope_left * this->settings.region_size;

                                if(index_left >= 0) {
                                    if(!region.map[index_left].tiletype.is_river() && !region.map[index_left]._marked) {
                                        const int elevation = smaller_slope(slope_left);  
                                        region.map[index_left]._marked = true;
                                        region.map[index_left].elevation = elevation;
                                        region.map[index_left].tiletype.set_terrain();
                                    }
                                }
                            }
                        }
                    }

                    // For the southern coast.
                    if(index + this->settings.region_size < this->getRegionSize()) {
                        if(!region.map[index + this->settings.region_size].tiletype.is_river()) {
                            for(int slope_right = 0; slope_right < terrain_slope; slope_right++) {
                                const int index_right = index + slope_right * this->settings.region_size;
                                
                                if(index_right < this->getRegionSize()) {
                                    if(!region.map[index_right].tiletype.is_river() && !region.map[index_right]._marked) {
                                        const int elevation = smaller_slope(slope_right);
                                        region.map[index_right]._marked = true;
                                        region.map[index_right].elevation = elevation;
                                        region.map[index_right].tiletype.set_terrain();
                                    }
                                }
                            }
                        }
                    }
                }

                // Mark the river tiles.
                region.map[index]._marked = true; 
            }

            // For tiles that are not placed along a river and are not the river itself.
            if(!region.map[index]._marked && !region.map[index].tiletype.is_river()) {
                // Terrain generation starts from the top left corner of the region
                // and continues until it reaches the bottom right corner.
                // That is why you only check only two cardinal directions, rather than four.
                int elevation_top  = (index >= this->settings.region_size)         ? region.map[index - this->settings.region_size].elevation * height_modifier : 0;
                int elevation_left = (index - 1 > 0)                               ? region.map[index - 1].elevation * height_modifier : 0;
                int elevation      = (index >= 0 && index < this->getRegionSize()) ? height[index] / 0.20f : 0;        

                int divisors  = 1;
                if(elevation_top != 0) {
                    elevation += elevation_top;
                    divisors++;
                }

                if(elevation_left != 0) {
                    elevation += elevation_left;
                    divisors++;
                }

                // Calculate the average value of the elevation of the three tiles.
                elevation /= divisors;
                
                region.map[index].elevation = elevation;
                region.map[index].tiletype.set_terrain();
            }   
        }
    }

    // Correct the position of tiles due to change in elevation.
    for(int i = 0; i < region.map.size(); i++) 
        region.map[i].object_position = region.map[i].object_position + sf::Vector2f(0, -region.map[i].elevation * this->settings.region_tile_size.y / 2);
}

int WorldGenerator::getWorldSize() const {
    return this->settings.world_size * this->settings.world_size;
}

int WorldGenerator::getRegionSize() const {
    return this->settings.region_size * this->settings.region_size;
}

/* Calculate index required for regionmap tile. */
int WorldGenerator::rCalculateIndex(int x, int y) const {
    return y * this->settings.region_size + x;
}

/* Calculate index required for worldmap region. */
int WorldGenerator::wCalculateIndex(int x, int y) const {
    return y * this->settings.world_size + x;
}

// This function returns the name of the tile type under certain index.
// Exact result is the name of a worldmap tile without type ID.
// For example: "panel_island" instead of "panel_island_0";
std::string WorldGenerator::getWorldmapTile(int index) const {
    auto& region    = this->world_map[index];
    auto is_terrain = region.regiontype.is_terrain();
    auto is_ocean   = region.regiontype.is_ocean();
    auto is_coast   = region.regiontype.is_coast();

    Biome biome = this->world_map[index].biome;

    if(!is_coast && (is_terrain || is_ocean)) {
        auto base_name = "panel_full";
        auto biome_specific_name = this->createBiomeSpecificTexture(base_name, biome);
        return biome_specific_name;
    }

    bool TOPLEFT     = false;
    bool TOP         = false;
    bool TOPRIGHT    = false;
    bool LEFT        = false;
    bool RIGHT       = false;
    bool BOTTOMLEFT  = false;
    bool BOTTOM      = false;
    bool BOTTOMRIGHT = false;

    auto world_size = this->getWorldSize();
    const auto in_limits = [world_size](int index) -> bool {
        return (index > 0 && index < world_size);
    };

    // LEFT
    if(in_limits(index - 1)) {
        if(this->world_map[index - 1].regiontype.is_terrain()) {
            LEFT = true;
        }
    }

    // RIGHT
    if(in_limits(index + 1)) {
        if(this->world_map[index + 1].regiontype.is_terrain()) {
            RIGHT = true;
        }
    }

    // TOP
    if(in_limits(index - this->settings.world_size)) {
        if(this->world_map[index - this->settings.world_size].regiontype.is_terrain()) {
            TOP = true;
        }
    }

    // TOP LEFT
    if(in_limits(index - 1 - this->settings.world_size)) {
        if(this->world_map[index - 1 - this->settings.world_size].regiontype.is_terrain()) {
            TOPLEFT = true;
        }
    }
    
    // TOP RIGHT
    if(in_limits(index + 1 - this->settings.world_size)) {
        if(this->world_map[index + 1 - this->settings.world_size].regiontype.is_terrain()) {
            TOPRIGHT = true;
        }
    }

    // BOTTOM
    if(in_limits(index + this->settings.world_size)) {
        if(this->world_map[index + this->settings.world_size].regiontype.is_terrain()) {
            BOTTOM = true;
        }
    }
    
    // BOTTOM LEFT
    if(in_limits(index - 1 + this->settings.world_size)) {
        if(this->world_map[index - 1 + this->settings.world_size].regiontype.is_terrain()) {
            BOTTOMLEFT = true;
        }
    }
    
    // BOTTOM RIGHT
    if(in_limits(index + 1 + this->settings.world_size)) {
        if(this->world_map[index + 1 + this->settings.world_size].regiontype.is_terrain()) {
            BOTTOMRIGHT = true;
        }
    }

    if(is_coast) {
        if(!LEFT && !TOP && !BOTTOM && !RIGHT && !TOPLEFT && !TOPRIGHT && !BOTTOMLEFT && !BOTTOMRIGHT) {
            auto base_name = this->getTileVariation("panel_island");;
            auto biome_specific_name = this->createBiomeSpecificTexture(base_name, biome);
            return biome_specific_name;
        }
        
        if(LEFT && TOP && BOTTOM && RIGHT && TOPLEFT && !BOTTOMRIGHT) {
            auto base_name = this->getTileVariation("panel_coast_turn_tl");;
            auto biome_specific_name = this->createBiomeSpecificTexture(base_name, biome);
            return biome_specific_name;   
        }

        if(LEFT && TOP && BOTTOM && RIGHT && BOTTOMLEFT && !TOPRIGHT) {
            auto base_name = this->getTileVariation("panel_coast_turn_bl");;
            auto biome_specific_name = this->createBiomeSpecificTexture(base_name, biome);
            return biome_specific_name;
        }

        if(LEFT && TOP && BOTTOM && RIGHT && TOPRIGHT && !BOTTOMLEFT) {
            auto base_name = this->getTileVariation("panel_coast_turn_tr");;
            auto biome_specific_name = this->createBiomeSpecificTexture(base_name, biome);
            return biome_specific_name;
        }

        if(LEFT && TOP && BOTTOM && RIGHT && BOTTOMRIGHT && !TOPLEFT) {
            auto base_name = this->getTileVariation("panel_coast_turn_br");;
            auto biome_specific_name = this->createBiomeSpecificTexture(base_name, biome);
            return biome_specific_name;
        }
        
        if(!LEFT && !TOP && BOTTOM && RIGHT) {
            auto base_name = this->getTileVariation("panel_coast_corner_tl");;
            auto biome_specific_name = this->createBiomeSpecificTexture(base_name, biome);
            return biome_specific_name;
        }
        
        if(!LEFT && TOP && !BOTTOM && RIGHT) {
            auto base_name = this->getTileVariation("panel_coast_corner_bl");;
            auto biome_specific_name = this->createBiomeSpecificTexture(base_name, biome);
            return biome_specific_name;  
        }

        if(LEFT && !TOP && BOTTOM && !RIGHT) {
            auto base_name = this->getTileVariation("panel_coast_corner_tr");;
            auto biome_specific_name = this->createBiomeSpecificTexture(base_name, biome);
            return biome_specific_name; 
        }

        if(LEFT && TOP && !BOTTOM && !RIGHT) {
            auto base_name = this->getTileVariation("panel_coast_corner_br");;
            auto biome_specific_name = this->createBiomeSpecificTexture(base_name, biome);
            return biome_specific_name;
        }
        
        if(BOTTOM && !TOP && LEFT && RIGHT) {
            auto base_name = this->getTileVariation("panel_coast_up");;
            auto biome_specific_name = this->createBiomeSpecificTexture(base_name, biome);
            return biome_specific_name;
        }

        if(!BOTTOM && TOP && LEFT && RIGHT) {
            auto base_name = this->getTileVariation("panel_coast_down");;
            auto biome_specific_name = this->createBiomeSpecificTexture(base_name, biome);
            return biome_specific_name;
        }

        if(!LEFT && RIGHT && BOTTOM && TOP) {
            auto base_name = this->getTileVariation("panel_coast_left");;
            auto biome_specific_name = this->createBiomeSpecificTexture(base_name, biome);
            return biome_specific_name;
        }

        if(LEFT && !RIGHT && BOTTOM && TOP) {
            auto base_name = this->getTileVariation("panel_coast_right");;
            auto biome_specific_name = this->createBiomeSpecificTexture(base_name, biome);
            return biome_specific_name;
        }
    }

    auto base_name = "panel_full";
    auto biome_specific_name = this->createBiomeSpecificTexture(base_name, biome);
    return biome_specific_name;
}

// Get a tile variation from "./res/worldmap/panel_atlas.png".
std::string WorldGenerator::getTileVariation(const std::string& id) const {
    // Default full panel does not have any texture variations at the moment.
    if(id == "panel_full")
        return "panel_full";

    // Rest of the tiles have 3 variations each.
    const int panel_type = rand() % 3;
    
    // Worldmap texture naming convention is: <panel_type><id>.
    // So the function returns for example: "panel_island_0".
    return id + "_" + std::to_string(panel_type);
}

// Use the texture templates from "./res/worldmap/panel_atlas.png" to create their biome specific variations.
std::string WorldGenerator::createBiomeSpecificTexture(const std::string& id, Biome biome) const {
    const std::string texture_id = id + "_" + toLower(biome.biome_name);

    if(this->resource->checkTextureExists(texture_id))
        return texture_id;

    auto image   = this->resource->getTexture(id).copyToImage();
    auto intrect = this->resource->getTextureIntRect(id);

    for(int y = 0; y < image.getSize().y; y++) {
        for(int x = 0; x < image.getSize().x; x++) {
            // Fill in terrain pixel.
            if(image.getPixel(x, y) == COLOUR_BLACK)
                image.setPixel(x, y, biome.biome_colour);
            
            // Fill in water pixel.
            else if(image.getPixel(x, y) == COLOUR_WHITE)
                image.setPixel(x, y, COLOUR_BLUE_OCEAN);
        }
    }

    sf::Texture texture;
    texture.loadFromImage(image);
    this->resource->addTexture(texture_id, texture, intrect);
    return texture_id;
}

std::string WorldGenerator::getWorldmapTreeTextureName(const Biome& biome) const {
    if(biome == BIOME_CONTINENTAL || biome == BIOME_TUNDRA)
        return "panel_tree_continental_1";

    if(biome == BIOME_TEMPERATE)
        return "panel_tree_temperate_1";

    if(biome == BIOME_MEADITERRANEAN || biome == BIOME_TROPICAL)
        return "panel_tree_mediterranean_1";

    return "default";
}
/*
std::string WorldGenerator::getRiverTile(int index) const {
    auto& region    = this->world_map[index];
    const auto is_terrain = region.regiontype.is_terrain();
    const auto is_ocean   = region.regiontype.is_ocean();
    const auto is_coast   = region.regiontype.is_coast();
    const auto is_forest  = region.regiontype.is_forest();

    bool TOPLEFT     = false;
    bool TOP         = false;
    bool TOPRIGHT    = false;
    bool LEFT        = false;
    bool RIGHT       = false;
    bool BOTTOMLEFT  = false;
    bool BOTTOM      = false;
    bool BOTTOMRIGHT = false;

    auto world_size = this->getWorldSize();
    const auto in_limits = [world_size](int index) -> bool {
        return (index > 0 && index < world_size);
    };

    // LEFT
    if(in_limits(index - 1)) {
        if(this->world_map[index - 1].regiontype.is_river()) {
            LEFT = true;
        }
    }

    // RIGHT
    if(in_limits(index + 1)) {
        if(this->world_map[index + 1].regiontype.is_river()) {
            RIGHT = true;
        }
    }

    // TOP
    if(in_limits(index - this->settings.world_size)) {
        if(this->world_map[index - this->settings.world_size].regiontype.is_river()) {
            TOP = true;
        }
    }

    // TOP LEFT
    if(in_limits(index - 1 - this->settings.world_size)) {
        if(this->world_map[index - 1 - this->settings.world_size].regiontype.is_river()) {
            TOPLEFT = true;
        }
    }
    
    // TOP RIGHT
    if(in_limits(index + 1 - this->settings.world_size)) {
        if(this->world_map[index + 1 - this->settings.world_size].regiontype.is_river()) {
            TOPRIGHT = true;
        }
    }

    // BOTTOM
    if(in_limits(index + this->settings.world_size)) {
        if(this->world_map[index + this->settings.world_size].regiontype.is_river()) {
            BOTTOM = true;
        }
    }
    
    // BOTTOM LEFT
    if(in_limits(index - 1 + this->settings.world_size)) {
        if(this->world_map[index - 1 + this->settings.world_size].regiontype.is_river()) {
            BOTTOMLEFT = true;
        }
    }
    
    // BOTTOM RIGHT
    if(in_limits(index + 1 + this->settings.world_size)) {
        if(this->world_map[index + 1 + this->settings.world_size].regiontype.is_river()) {
            BOTTOMRIGHT = true;
        }
    }

    if(TOP && LEFT && !RIGHT)
        return this->getRiverTileVariation("panel_river_turn_br");

    if(TOP && RIGHT && !LEFT)
        return this->getRiverTileVariation("panel_river_turn_bl");

    if(BOTTOM && LEFT && !RIGHT)
        return this->getRiverTileVariation("panel_river_turn_tr");

    if(BOTTOM && RIGHT && !LEFT)
        return this->getRiverTileVariation("panel_river_turn_tl");

    // If the horizontal or vertical tile is a coastal tile, 
    // the texture returned will be a estuary to point out it's a river delta.
    
    if(LEFT && RIGHT)
        return this->getRiverTileVariation("panel_river_horizontal");

    if(in_limits(index - 1))
        if(this->is_ocean(index - 1) && RIGHT)
            return this->getRiverTileVariation("panel_river_estuary_left");
        
        if(in_limits(index + 1))
            if(this->is_ocean(index + 1) && LEFT)
                return this->getRiverTileVariation("panel_river_estuary_right");

    if(TOP && BOTTOM)
        return this->getRiverTileVariation("panel_river_vertical"); 
    
    if(in_limits(index - this->settings.world_size))
        if(this->is_ocean(index - this->settings.world_size) && BOTTOM)
            return this->getRiverTileVariation("panel_river_estuary_up");

    if(in_limits(index + this->settings.world_size))
        if(this->is_ocean(index + this->settings.world_size) && TOP)
            return this->getRiverTileVariation("panel_river_estuary_down");
 
    return "panel_full_lightblue";
}
*/

std::string WorldGenerator::getRiverTileVariation(const std::string& id) const {
    // Rest of the tiles have 3 variations each.
    const int panel_type = rand() % 3;
    
    // Worldmap texture naming convention is: <panel_type><id>.
    // So the function returns for example: "panel_river_horizontal_0".
    return id + "_" + std::to_string(panel_type);
}

std::string WorldGenerator::createColouredTexture(const std::string& id, const std::string& save_as, const sf::Color colour_main, const sf::Color colour_secondary) const {
    if(this->resource->checkTextureExists(save_as))
        return save_as;

    auto image   = this->resource->getTexture(id).copyToImage();
    auto intrect = this->resource->getTextureIntRect(id);

    for(int y = 0; y < image.getSize().y; y++) {
        for(int x = 0; x < image.getSize().x; x++) {
            // Fill in terrain pixel.
            if(image.getPixel(x, y) == COLOUR_BLACK)
                image.setPixel(x, y, colour_main);
            
            // Fill in water pixel.
            else if(image.getPixel(x, y) == COLOUR_WHITE)
                image.setPixel(x, y, colour_secondary);
        }
    }

    sf::Texture texture;
    texture.loadFromImage(image);
    this->resource->addTexture(save_as, texture, intrect);
    return save_as;
}