#include "worldGenerator.hpp"

using namespace iso;

worldGenerator::worldGenerator() {

}

worldGenerator::worldGenerator(WorldSettings& world_settings, RegionSettings& region_settings, entropy::Entropy* engine) {
    this->world_settings  = world_settings;
    this->region_settings = region_settings;
    this->m_engine        = engine;
    this->m_log_prefix    = "[World Generation]";

    const int world_size = this->world_settings.size.x * this->world_settings.size.y;

    this->m_noise.resize(world_size);
    this->m_gradient.resize(world_size);
    this->world_map.resize(world_size);

    std::srand(std::time(0));
}

worldGenerator::~worldGenerator() {

}

void worldGenerator::generateWorld() {
    std::cout << this->m_log_prefix << ": Generating world.\n";
    
    sf::Clock clock;
    unsigned int panel_quantity = 0;

    std::cout << this->m_log_prefix << ": Generating noise.\n";

    this->generateNoiseMap();
    this->generateCircularGradient();

    std::cout << this->m_log_prefix << ": Generating terrain.\n";

    for(int i = 0; i < this->world_settings.size.x * this->world_settings.size.y; i++) {
        this->world_map[i] = Region();
    }

    for(int x = 0; x < this->world_settings.size.x; x++) {
        for(int y = 0; y < this->world_settings.size.y; y++) {                
            const int index = x + y * this->world_settings.size.x;
            const float noise    = this->m_noise[index]; 
            const float gradient = this->m_gradient[index];
            float value          = noise - gradient;

            if(value > this->world_settings.minimum_terrain_height) {
                this->m_region.regiontype.set_terrain();
                panel_quantity++;
            } else this->m_region.regiontype.set_ocean();

            if(value < 0.0f) value = 0.0f;
            if(value > 1.0f) value = 1.0f;

            this->m_region.height     = value;
            this->m_region.position.x = x * this->world_settings.panel_size.x; 
            this->m_region.position.y = y * this->world_settings.panel_size.y;
            this->m_region.size       = this->world_settings.panel_size;

            this->world_map[index] = this->m_region;
        }
    }

    this->generatePoles();

    // Find and mark coast tiles.
    for(int x = 0; x < this->world_settings.size.x; x++) {
        for(int y = 0; y < this->world_settings.size.y; y++) {
            const int index = x + y * this->world_settings.size.x;
            
            if(this->world_map[index].regiontype.is_terrain()) {
                const int index_up    = index - this->world_settings.size.x;             
                const int index_down  = index + this->world_settings.size.x;
                const int index_left  = index - 1;
                const int index_right = index + 1;

                if(index_up > 0)
                    if(!this->world_map[index_up].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();

                if(index_left > 0)
                    if(!this->world_map[index_left].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();

                if(index_down < this->world_settings.size.x * this->world_settings.size.y)
                    if(!this->world_map[index_down].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();

                if(index_right < this->world_settings.size.x * this->world_settings.size.y)
                    if(!this->world_map[index_right].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();
            }
        }
    }

    std::cout << this->m_log_prefix << ": Simulating world climate.\n";
    
    this->generateLatititude();
    this->generateTemperature();
    this->generateMoistureMap();
    this->assignBiome();

    std::cout << this->m_log_prefix << ": Generating world features.\n";

    this->generateRivers();
    this->generateForests();

    std::cout << this->m_log_prefix << ": World generated.\n";
    std::cout << this->m_log_prefix << ": Terrain is " << panel_quantity  << " panels out of total " << this->world_settings.size.x * this->world_settings.size.y << " panels" << " (" << int(float(panel_quantity / float(this->world_settings.size.x * this->world_settings.size.y)) * 100) << "%)." << "\n";
    std::cout << this->m_log_prefix << ": " << this->world_settings.size.x * this->world_settings.size.y << " panels generated in " << clock.getElapsedTime().asSeconds() << " seconds.\n";
}

void worldGenerator::generateNoiseMap() {
    std::vector <float> input(this->world_settings.size.x * this->world_settings.size.y);
    for(int i = 0; i < this->world_settings.size.x * this->world_settings.size.y; i++) {
        input[i] = (float)rand() / (float)RAND_MAX;    
    }

    float biggest_noise_recorded = 1.0f;

    for(int x = 0; x < this->world_settings.size.x; x++) {
        for(int y = 0; y < this->world_settings.size.y; y++) {
            const int index = x + y * this->world_settings.size.x;

            if(x < this->world_settings.margin.x || x > (this->world_settings.size.x - this->world_settings.margin.x) || y < this->world_settings.margin.y || y > (this->world_settings.size.y - this->world_settings.margin.y)) {
                this->m_noise[index] = 0.0f;
                continue;
            }

            float noise = 0.0f;
            float scale = 1.0f;
            float scale_acc = 0.0f;

            for(unsigned int o = 0; o < this->world_settings.noise_octaves; o++) {
                const int sampleX1 = (x / this->world_settings.noise_persistence) * this->world_settings.noise_persistence;
                const int sampleY1 = (y / this->world_settings.noise_persistence) * this->world_settings.noise_persistence;

                const int sampleX2 = (sampleX1 + this->world_settings.noise_persistence) % (int)this->world_settings.size.x;					
                const int sampleY2 = (sampleY1 + this->world_settings.noise_persistence) % (int)this->world_settings.size.x;

                const float blendX = (float)(x - sampleX1) / (float)this->world_settings.noise_persistence;
                const float blendY = (float)(y - sampleY1) / (float)this->world_settings.noise_persistence;

                const float sampleT = (1.0f - blendX) * input[sampleY1 * this->world_settings.size.x + sampleX1] + blendX * input[sampleY1 * this->world_settings.size.x + sampleX2];
                const float sampleB = (1.0f - blendX) * input[sampleY2 * this->world_settings.size.x + sampleX1] + blendX * input[sampleY2 * this->world_settings.size.x + sampleX2];

                scale_acc += scale;
                noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
                scale = float(scale / this->world_settings.noise_bias);
            }
            
            float noise_value = (noise / scale_acc) * this->world_settings.multiplier_noise;
            
            if(noise_value > biggest_noise_recorded) biggest_noise_recorded = noise_value;

            noise_value *= biggest_noise_recorded;
            if(noise_value > 1.0f) noise_value = 1.0f;

            this->m_noise[index] = noise_value;
        }
    }
}

void worldGenerator::generateCircularGradient() {
    const sf::Vector2i centre = sf::Vector2i(
        this->world_settings.size.x / 2,
        this->world_settings.size.y / 2
    );

    for(int x = 0; x < this->world_settings.size.x; x++) {
        for(int y = 0; y < this->world_settings.size.y; y++) {
            const int index = x + y * this->world_settings.size.x;

            if(x < this->world_settings.margin.x || x > (this->world_settings.size.x - this->world_settings.margin.x) || y < this->world_settings.margin.y || y > (this->world_settings.size.y - this->world_settings.margin.y)) {
                this->m_gradient[index] = 0.0f;
                continue;
            }

            const float distance_x = (centre.x - x) * (centre.x - x);
            const float distance_y = (centre.y - y) * (centre.y - y);

            // Both width and height of the map have a impact on distance.
            float distance = (sqrt(distance_x + distance_y) / (0.5f * (this->world_settings.size.x + this->world_settings.size.y)));

            if(distance > 1.0f) distance = 1.0f;

            // Distance is bigger when further away from the centre of the gradient.
            this->m_gradient[index] = distance * this->world_settings.multiplier_gradient;
        }
    }
}

void worldGenerator::generatePoles() {
    std::cout << this->m_log_prefix << ": Generating poles.\n";
    
    const float chance   = 1.0f;
    const float modifier = 0.6f;

    for(unsigned int x = 0; x < this->world_settings.size.x; x++) {
        float pole_chance   = chance;
        float pole_modifier = modifier;

        for(unsigned int y = 0; y < this->world_settings.pole_size; y++) {
            const int   index = y * this->world_settings.size.x + x;
            const float random_number1 = (float)rand();
            const float random_number2 = (float)RAND_MAX * pole_chance;

            if(random_number1 < random_number2) {
                this->world_map[index].regiontype.set_terrain();
                this->world_map[index].biome = BIOME_ARCTIC;
            }
        
            pole_chance *= pole_modifier;
        }
    }

    for(unsigned int x = 0; x < this->world_settings.size.x; x++) { 
        float pole_chance   = chance;
        float pole_modifier = modifier;

        for(unsigned int y = this->world_settings.size.y - 1; y > this->world_settings.size.y - 1 - this->world_settings.pole_size; y--) {
            const int   index = y * this->world_settings.size.x + x;
            const float random_number1 = (float)rand();
            const float random_number2 = (float)RAND_MAX * pole_chance;

            if(random_number1 < random_number2) {
                this->world_map[index].regiontype.set_terrain();
                this->world_map[index].biome = BIOME_ARCTIC;
            }
            
            pole_chance *= pole_modifier;
        }

        pole_chance   = chance;
        pole_modifier = modifier;
    }
}

void worldGenerator::generateRivers() {
    std::cout << this->m_log_prefix << ": Generating rivers.\n";
    
    // Number of rivers is a maximum amount of rivers.
    // It does not mean that there will be the specified amount.

    // Storage for indexes of already created rivers.
    std::vector <int> river_origin_index(this->world_settings.river_quantity);
    
    for(unsigned int river_number = 0; river_number < this->world_settings.river_quantity; river_number++) {
        int possible_river_origin_index = rand() % this->world_map.size();
        bool place_found = false;

        while(!place_found) {
            int index = rand() % this->world_map.size() - 1;
            
            if(this->world_map[index].regiontype.is_terrain() && !this->world_map[index].regiontype.is_coast() && !this->is_arctic(index) && !this->world_map[index].regiontype.is_forest()) {
                possible_river_origin_index = index;
                place_found = true;
            }
        }

        // Area (Panels) that should be scanned in each geographical direction from the possible river origin position. 
        const int area_around_origin = (this->world_settings.size.x + this->world_settings.size.y) / 2 / 15;
        unsigned int rivers_verified = 0;
        
        // Scan the provided area around the possible origin of the river to check if it is suitable.
        // The area is suitable if there are no rivers nearby.             
        for(unsigned int scanned_river = 0; scanned_river < river_origin_index.size(); scanned_river++) {
            int panels_verified = 0;
            int panels_occupied = 0;
            
            for(int x = 0 - area_around_origin; x <= 0 + area_around_origin; x++) {
                for(int y = 0 - area_around_origin; y <= 0 + area_around_origin; y++) {
                    const int index = possible_river_origin_index + (x + y * this->world_settings.size.y);

                    if(index != river_origin_index[scanned_river]) {
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
        if(rivers_verified == this->world_settings.river_quantity) {
            river_index = possible_river_origin_index;
            river_origin_index[river_number] = possible_river_origin_index;
        }
        
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

            const int index_up    = river_index_current - this->world_settings.size.x;
            const int index_down  = river_index_current + this->world_settings.size.x;
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

            RiverDirection direction = RiverDirection::RIVER_ORIGIN;

            if(river_index_current != river_index_start) {
                if(current_move_direction == index_up) {
                    // UP AND RIGHT
                    if(current_move_value + last_move_value == -this->world_settings.size.x + 1) {
                        direction = RiverDirection::RIVER_NORTH_TO_WEST;
                    }

                    // UP AND LEFT
                    if(current_move_value + last_move_value == -this->world_settings.size.x - 1) {
                        direction = RiverDirection::RIVER_NORTH_TO_EAST;
                    }

                    // UP
                    if(current_move_value == -this->world_settings.size.x && last_move_value == -this->world_settings.size.x) {
                        direction = RiverDirection::RIVER_VERTICAL;
                    }

                }

                else if(current_move_direction == index_down) {
                    // DOWN AND RIGHT
                    if(current_move_value + last_move_value == this->world_settings.size.x + 1) {
                        direction = RiverDirection::RIVER_SOUTH_TO_WEST;
                    }

                    // DOWN AND LEFT
                    if(current_move_value + last_move_value == this->world_settings.size.x - 1) {
                        direction = RiverDirection::RIVER_SOUTH_TO_EAST;
                    }

                    // DOWN
                    if(current_move_value == this->world_settings.size.x && last_move_value == this->world_settings.size.x) {
                        direction = RiverDirection::RIVER_VERTICAL;
                    }
                }

                else if(current_move_direction == index_left) {
                    // LEFT
                    if(current_move_value + last_move_value == -1 || -2) {
                        direction = RiverDirection::RIVER_HORIZONTAL;
                    }

                    // LEFT AND UP
                    if(current_move_value + last_move_value == -1 - this->world_settings.size.x) {
                        direction = RiverDirection::RIVER_SOUTH_TO_WEST;
                    }

                    // LEFT AND DOWN
                    if(current_move_value + last_move_value == -1 + this->world_settings.size.x) {
                        direction = RiverDirection::RIVER_NORTH_TO_WEST;
                    }

                }

                else if(current_move_direction == index_right) {
                    // RIGHT
                    if(current_move_value + last_move_value == 1 || 2) {
                        direction = RiverDirection::RIVER_HORIZONTAL;
                    }

                    // RIGHT AND UP
                    if(current_move_value + last_move_value == 1 - this->world_settings.size.x) {
                        direction = RiverDirection::RIVER_SOUTH_TO_EAST;
                    }

                    // RIGHT AND DOWN
                    if(current_move_value + last_move_value == 1 + this->world_settings.size.x) {
                        direction = RiverDirection::RIVER_NORTH_TO_EAST;
                    }
                }

                else 
                    direction = RiverDirection::RIVER_NONE; 
            }

            Region& panel = this->world_map[river_index_current];
            const sf::Vector2f panel_position = panel.position;
            const sf::Vector2f panel_offset   = sf::Vector2f(0, 0); 
            const sf::Vector2f panel_size     = this->world_settings.panel_size; 

            switch(direction) {
                case RiverDirection::RIVER_NONE:
                    panel._direction = RiverDirection::RIVER_NONE;
                    break;

                case RiverDirection::RIVER_ORIGIN:
                    panel._direction = RiverDirection::RIVER_ORIGIN;
                    panel.river = GameObject(panel_position, panel_offset, panel_size, &this->m_engine->resource.getTexture("panel_sea")); 
                    break;
                
                case RiverDirection::RIVER_NORTH_TO_EAST:
                    panel._direction = RiverDirection::RIVER_NORTH_TO_EAST;
                    panel.river = GameObject(panel_position, panel_offset, panel_size, &this->m_engine->resource.getTexture("panel_river_corner_bl")); 
                    break;

                case RiverDirection::RIVER_NORTH_TO_WEST:
                    panel._direction = RiverDirection::RIVER_NORTH_TO_WEST;
                    panel.river = GameObject(panel_position, panel_offset, panel_size, &this->m_engine->resource.getTexture("panel_river_corner_br")); 
                    break;

                case RiverDirection::RIVER_SOUTH_TO_EAST:
                    panel._direction = RiverDirection::RIVER_SOUTH_TO_EAST;
                    panel.river = GameObject(panel_position, panel_offset, panel_size, &this->m_engine->resource.getTexture("panel_river_corner_tl")); 
                    break;

                case RiverDirection::RIVER_SOUTH_TO_WEST:
                    panel._direction = RiverDirection::RIVER_SOUTH_TO_WEST;
                    panel.river = GameObject(panel_position, panel_offset, panel_size, &this->m_engine->resource.getTexture("panel_river_corner_tr")); 
                    break;

                case RiverDirection::RIVER_VERTICAL:
                    panel._direction = RiverDirection::RIVER_VERTICAL;
                    panel.river = GameObject(panel_position, panel_offset, panel_size, &this->m_engine->resource.getTexture("panel_river_vertical")); 
                    break;

                case RiverDirection::RIVER_HORIZONTAL:
                    panel._direction = RiverDirection::RIVER_HORIZONTAL;
                    panel.river = GameObject(panel_position, panel_offset, panel_size, &this->m_engine->resource.getTexture("panel_river_horizontal")); 
                    break;
            }

            panel.regiontype.set_river();

            

            last_move_value = current_move_value;
            river_index_current = current_move_direction;

            if(coast_found)
                break;

            if(this->world_map[river_index_current].regiontype.is_coast())
                coast_found = true;
        }
    }
}

void worldGenerator::generateLatititude() {
    std::cout << this->m_log_prefix << ": Calculating latitude.\n";
    
    for(int x = 0; x < this->world_settings.size.x; x++) {
        for(int y = 0; y < this->world_settings.size.y; y++) {       
            const int index = x + y * this->world_settings.size.x;

            // Latitude for the upper half.
            if(y < this->world_settings.size.y / 2)
                this->world_map[index].latitude = float(y + 1) / float(this->world_settings.size.y / 2);

            // Latitude for the bottom half.
            else {
                const int i = y - this->world_settings.size.y / 2;
                const int reversed_height = y - 2 * i;
                this->world_map[index].latitude = float(reversed_height) / float(this->world_settings.size.y / 2);
            }
        }
    }
}

void worldGenerator::generateTemperature() {
    std::cout << this->m_log_prefix << ": Calculating temperature.\n";
    
    for(int x = 0; x < this->world_settings.size.x; x++) {
        for(int y = 0; y < this->world_settings.size.y; y++) {            
            const int panel_index = y * this->world_settings.size.x + x;
            Region& panel = this->world_map[panel_index];

            // Panel temperature zone classification.
            const bool PANEL_ARCTIC       = (y < this->world_settings.pole_size || y > this->world_settings.size.y - this->world_settings.pole_size - 1) ? true : false;
            const bool PANEL_NEAR_POLE    = ((y < this->world_settings.pole_size + this->world_settings.margin.y + 0.1f * (float)this->world_settings.size.y && y >= this->world_settings.pole_size) || (y >= this->world_settings.size.y - (this->world_settings.pole_size + this->world_settings.margin.y + 0.1f * (float)this->world_settings.size.y) && y < this->world_settings.size.y - this->world_settings.pole_size)) ? true : false;
            const bool PANEL_NEAR_EQUATOR = (y > this->world_settings.size.y / 2 - 1 - this->world_settings.margin.y && y <= this->world_settings.size.y / 2 + 1 + this->world_settings.margin.y) ? true : false;

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
}

void worldGenerator::generateMoistureMap() {
    std::cout << this->m_log_prefix << ": Calculating moisture.\n";
    
    std::vector <float> input(this->world_settings.size.x * this->world_settings.size.y);
    for(int i = 0; i < this->world_settings.size.x * this->world_settings.size.y; i++) {
        input[i] = (float)rand() / (float)RAND_MAX;
    }
    
    float biggest_noise_recorded = 1.0f;

    for(int x = 0; x < this->world_settings.size.x; x++) {
        for(int y = 0; y < this->world_settings.size.y; y++) {
            const int index = y * this->world_settings.size.x + x;

            if(!this->world_map[index].regiontype.is_terrain())
                this->world_map[index].moisture = 1.0f;

            else if(this->is_arctic(index))
                this->world_map[index].moisture = 0.0f;

            else if(this->world_map[index].regiontype.is_terrain() && !this->is_arctic(index)) {
                float noise = 0.0f;
                float scale = 1.0f;
                float scale_acc = 0.0f;

                for(unsigned int o = 0; o < this->world_settings.moisture_octaves; o++) {
                    const int sampleX1 = (x / this->world_settings.moisture_persistence) * this->world_settings.moisture_persistence;
                    const int sampleY1 = (y / this->world_settings.moisture_persistence) * this->world_settings.moisture_persistence;

                    const int sampleX2 = (sampleX1 + this->world_settings.moisture_persistence) % (int)this->world_settings.size.x;					
                    const int sampleY2 = (sampleY1 + this->world_settings.moisture_persistence) % (int)this->world_settings.size.x;

                    const float blendX = (float)(x - sampleX1) / (float)this->world_settings.moisture_persistence;
                    const float blendY = (float)(y - sampleY1) / (float)this->world_settings.moisture_persistence;

                    const float sampleT = (1.0f - blendX) * input[sampleY1 * this->world_settings.size.x + sampleX1] + blendX * input[sampleY1 * this->world_settings.size.x + sampleX2];
                    const float sampleB = (1.0f - blendX) * input[sampleY2 * this->world_settings.size.x + sampleX1] + blendX * input[sampleY2 * this->world_settings.size.x + sampleX2];

                    scale_acc += scale;
                    noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
                    scale = float(scale / this->world_settings.moisture_bias);
                }
                
                float noise_value = (noise / scale_acc) * this->world_settings.multiplier_moisture;
                
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

                this->world_map[y * this->world_settings.size.x + x].moisture = noise_value;
            }
        }
    }
}

void worldGenerator::assignBiome() {
    std::cout << this->m_log_prefix << ": Generating biomes.\n";
    
    for(int index = 0; index < this->world_settings.size.x * this->world_settings.size.y; index++) {
        Region& panel = this->world_map[index];

        if(!panel.regiontype.is_terrain()) {
            panel.texture = this->m_engine->resource.getTexture("panel_ocean");
            panel.biome   = BIOME_OCEAN;
        } 
        
        else if(this->is_arctic(index)) {
            panel.texture = this->m_engine->resource.getTexture("panel_arctic");
            panel.biome   = BIOME_ARCTIC;
        } 
    
        else if(panel.temperature < 0.45f && panel.latitude < 0.5f) {    
            if(panel.moisture > 0.23f) {
                panel.texture = this->m_engine->resource.getTexture("panel_grass_cold");
                panel.biome   = BIOME_CONTINENTAL;
            }       
            
            else {
                panel.texture = this->m_engine->resource.getTexture("panel_tundra");
                panel.biome   = BIOME_TUNDRA;
            } 
        }

        else if(panel.temperature < 0.55f && panel.latitude < 0.55f) {
            const int tiletype = rand() % 10;

            if(tiletype > 5) {
                panel.texture = this->m_engine->resource.getTexture("panel_grass_warm");
                panel.biome   = BIOME_TEMPERATE;
            }   

            else if(tiletype > 2) {
                panel.texture = this->m_engine->resource.getTexture("panel_grass_cold");
                panel.biome   = BIOME_CONTINENTAL;
            }
            
            else {
                panel.texture = this->m_engine->resource.getTexture("panel_tundra");
                panel.biome   = BIOME_TUNDRA;
            } 
        }

        else if(panel.temperature < 0.7f) {
            panel.texture = this->m_engine->resource.getTexture("panel_grass_warm"); 
            panel.biome   = BIOME_TEMPERATE;
        }

        else {
            if(panel.moisture > 0.51f) {
                panel.texture = this->m_engine->resource.getTexture("panel_grass_tropical");
                panel.biome   = BIOME_TROPICAL;
            }

            else if(panel.moisture > 0.23f) {
                panel.texture = this->m_engine->resource.getTexture("panel_grass_subtropical");    
                panel.biome   = BIOME_MEADITERRANEAN;
            }

            // Currently there is no purpose for this biome, so why add it to the terrain generator?
            // else if(panel->moisture > 0.04f && panel->moisture < 0.10f) {
            //     panel->texture = this->m_engine->resource.getTexture("panel_desert");
            //     panel->biome = BIOME_DESERT;
            // } 

            else {
                panel.texture = this->m_engine->resource.getTexture("panel_grass_warm");
                panel.biome   = BIOME_TEMPERATE;
            }
        } 
    }
}

void worldGenerator::generateForests() {
    std::cout << this->m_log_prefix << ": Generating forests.\n";

    noiseSettings settings;
    settings.octaves     = 8;
    settings.bias        = 4;
    settings.persistence = 4;
    settings.multiplier  = 1.25f;
    settings.size        = this->world_settings.size;

    this->generateNoise(settings, this->m_tree_noise);

    for(int index = 0; index < this->m_tree_noise.size(); index++) {
        const float value    = this->m_tree_noise[index];
        const float noise    = (float)rand() / (float)RAND_MAX;
        const float combined = value / 2 + noise / 2;
        const float result   = (combined > 1.0f) ? 1.0f : combined;

        Region& region = this->world_map[index];

        if(result > 0.7f && !this->is_arctic(index) && !this->is_desert(index) && !this->world_map[index].regiontype.is_river() && this->world_map[index].regiontype.is_terrain()) {
            region.forest = GameObject(region.position, sf::Vector2f(0, 0), region.size, &this->getTreeTextureWorld(region.biome)); 
	        region.regiontype.set_forest();
	    }
    }    
}

void worldGenerator::generateRegion(int index, Region& region) {
    sf::Clock clock;

    // Do this because if at least one region was visitied,
    // the m_tile property still holds value from the last region generation.
    this->m_tile = Tile();

    this->m_tile.size    = this->region_settings.tile_size;
    this->m_tile.texture = this->getBiomeTileTexture(region.biome);

    // Create a flat map populated with tiles on the first elevation layer (0-th).
    region.map.resize(this->region_settings.size.x * this->region_settings.size.y);
    for(int y = 0; y < this->region_settings.size.y; y++) {
        for(int x = 0; x < this->region_settings.size.x; x++) {
            const int index       = y * this->world_settings.size.x + x;
            this->m_tile.position = this->tilePositionScreen(x, y);
            region.map[index]     = this->m_tile;
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
            for(int y = 0; y < this->region_settings.size.y; y++) {
                for(int x = 0; x < this->region_settings.size.x; x++) {
                    const int index = y * this->region_settings.size.x + x;
    
                    if(region.map[index].tiletype.is_river()) {
                        region.map[index].elevation = 0;
                        region.map[index].position  = this->tilePositionScreen(x, y);
                    }
                }
            }
        }
    }

    // Make sure that high tiles have sides.
    for(int y = 0; y < this->region_settings.size.y; y++) {
        for(int x = 0; x < this->region_settings.size.x; x++) {
            const int index = y * this->region_settings.size.x + x;

            // Indexes to check.
            const int index_bottom = index + this->region_settings.size.x;
            const int index_right  = index + 1;
            const int index_left   = index - 1;
            
            // To avoid blank spots, you only fill in the missing height, when the biggest slope was found.
            int biggest_difference = 0;

            if(index_bottom < this->region_settings.size.x * this->region_settings.size.y) {
                if(region.map[index].elevation > region.map[index_bottom].elevation) {
                    const int height_difference = std::abs(region.map[index].elevation - region.map[index_bottom].elevation);
                    if(biggest_difference < height_difference)
                        biggest_difference = height_difference;
                }
            }

            if(index_right < this->region_settings.size.x * this->region_settings.size.y) {
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

            region.map[index].side.resize(biggest_difference);
            for(int i = 0; i < biggest_difference; i++) {
                std::string tile = i > 2 ? "tile_height_stone" : "tile_height_dirt";
                region.map[index].side[i] = GameObject(region.map[index].position, sf::Vector2f(0, this->region_settings.tile_size.y / 2 + i * this->region_settings.tile_size.y / 2), this->region_settings.tile_size, &this->m_engine->resource.getTexture(tile));
            }

            // Make sure that the border tiles have depth.
            // Tiles near the y edge.
            if(y == this->region_settings.size.y - 1) {
                const int height_difference = std::abs(0 - region.map[(this->region_settings.size.y - 1) * this->region_settings.size.x + x].elevation) + 1;
                region.map[index].side.resize(height_difference);
                
                for(int i = 0; i < height_difference; i++) {
                    std::string tile = i > 2 ? "tile_height_stone" : "tile_height_dirt";
                    region.map[index].side[i] = GameObject(region.map[index].position, sf::Vector2f(0, this->region_settings.tile_size.y / 2 + i * this->region_settings.tile_size.y / 2), this->region_settings.tile_size, &this->m_engine->resource.getTexture(tile));
                }
            }

            // Tiles near the x edge.
            if(x == this->region_settings.size.x - 1) {
                const int height_difference = std::abs(0 - region.map[y * this->region_settings.size.x + this->region_settings.size.x - 1].elevation) + 1;
                region.map[index].side.resize(height_difference);
                
                for(int i = 0; i < height_difference; i++) {
                    std::string tile = i > 2 ? "tile_height_stone" : "tile_height_dirt";
                    region.map[index].side[i] = GameObject(region.map[index].position, sf::Vector2f(0, this->region_settings.tile_size.y / 2 + i * this->region_settings.tile_size.y / 2), this->region_settings.tile_size, &this->m_engine->resource.getTexture(tile));
                }
            }
        }
    }

    region.visited = true;

    std::cout << "[World Generation][Region Generation]: Region " << index << " generated in " << clock.getElapsedTime().asSeconds() << " seconds.\n";
}

void worldGenerator::generateNoise(noiseSettings& settings, std::vector<float>& storage) {
    // Generate a random input sequence.
    std::vector <float> input(settings.size.x * settings.size.y);
    for(int i = 0; i < input.size(); i++) {
        input[i] = (float)rand() / (float)RAND_MAX;
    }

    storage.resize(settings.size.x * settings.size.y);

    float amplifier = 1.0f;

    // Generate the noise.
    for(unsigned int x = 0; x < settings.size.x; x++) {
        for(unsigned int y = 0; y < settings.size.y; y++) {
            float noise = 0.0f;       // Noise accumulator.
            float scale = 1.0f;       // The frequency of the noise.
            float scale_check = 0.0f; // Scale check to keep the noise values between 0 and 1.

            for(unsigned int o = 0; o < settings.octaves; o++) {
                const int sampleX1 = (x / settings.persistence) * settings.persistence;
                const int sampleY1 = (y / settings.persistence) * settings.persistence;
                
                const int sampleX2 = (sampleX1 + settings.persistence) % (int)settings.size.x;					
                const int sampleY2 = (sampleY1 + settings.persistence) % (int)settings.size.x;
                
                const float blendX = (float)(x - sampleX1) / (float)settings.persistence;
                const float blendY = (float)(y - sampleY1) / (float)settings.persistence;
                
                const float sampleT = (1.0f - blendX) * input[sampleY1 * (int)settings.size.x + sampleX1] + blendX * input[sampleY1 * (int)settings.size.x + sampleX2];
                const float sampleB = (1.0f - blendX) * input[sampleY2 * (int)settings.size.x + sampleX1] + blendX * input[sampleY2 * (int)settings.size.x + sampleX2];
            
                scale_check += scale;
                noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
                scale = float(scale / settings.bias);
            }

            const int index = y * settings.size.x + x;

            float buffer_noise = (noise / scale_check) * settings.multiplier;
            
            if(buffer_noise > amplifier) amplifier = buffer_noise;
            buffer_noise *= amplifier;

            storage[index] = (buffer_noise > 1.0f) ? 1.0f : buffer_noise; 
        }
    }
}

sf::Texture& worldGenerator::getBiomeTileTexture(Biome biome) {
    if(biome.biome_name == "Continental")
        return this->m_engine->resource.getTexture("tile_grass_cold");

    else if(biome.biome_name == "Temperate")    
        return this->m_engine->resource.getTexture("tile_grass_warm");
        
    else if(biome.biome_name == "Tropical")
        return this->m_engine->resource.getTexture("tile_grass_tropical");

    else if(biome.biome_name == "Mediterranean")
        return this->m_engine->resource.getTexture("tile_grass_subtropical");

    else if(biome.biome_name == "Ocean")
        return this->m_engine->resource.getTexture("tile_ocean");

    else if(biome.biome_name == "Sea")
        return this->m_engine->resource.getTexture("tile_sea");

    else if(biome.biome_name == "Tundra")
        return this->m_engine->resource.getTexture("tile_tundra");

    else if(biome.biome_name == "Arctic")
        return this->m_engine->resource.getTexture("tile_arctic");

    else if(biome.biome_name == "Desert")
        return this->m_engine->resource.getTexture("tile_desert");

    else return this->m_engine->resource.getTexture("default");
}

sf::Vector2f worldGenerator::tilePositionScreen(int x, int y) {
    return sf::Vector2f(
        (this->region_settings.tile_offset.x * this->region_settings.tile_size.x) + (x - y) * (this->region_settings.tile_size.x / 2),
        (this->region_settings.tile_offset.y * this->region_settings.tile_size.y) + (x + y) * (this->region_settings.tile_size.y / 2)
    );
}

sf::Vector2f worldGenerator::tilePositionScreen(sf::Vector2i tile_position) {
    return this->tilePositionScreen(tile_position.x, tile_position.y);
}

sf::Vector2f worldGenerator::tilePositionScreen(sf::Vector2f tile_position) {
    return this->tilePositionScreen(sf::Vector2i(tile_position.x, tile_position.y));
}

bool worldGenerator::is_biome(int region_index, Biome biome) {
    if(this->world_map[region_index].biome.biome_name == biome.biome_name)
        return true;
    return false;
}

bool worldGenerator::is_arctic(int region_index) {
    return this->is_biome(region_index, BIOME_ARCTIC);
}

bool worldGenerator::is_ocean(int region_index) {
    return this->is_biome(region_index, BIOME_OCEAN);
}

bool worldGenerator::is_sea(int region_index) {
    return this->is_biome(region_index, BIOME_SEA);
}

bool worldGenerator::is_tropical(int region_index) {
    return this->is_biome(region_index, BIOME_TROPICAL);
}

bool worldGenerator::is_mediterranean(int region_index) {
    return this->is_biome(region_index, BIOME_MEADITERRANEAN);
}

bool worldGenerator::is_temperate(int region_index) {
    return this->is_biome(region_index, BIOME_TEMPERATE);
}

bool worldGenerator::is_continental(int region_index) {
    return this->is_biome(region_index, BIOME_CONTINENTAL);
}

bool worldGenerator::is_tundra(int region_index) {
    return this->is_biome(region_index, BIOME_TUNDRA);
}

bool worldGenerator::is_desert(int region_index) {
    return this->is_biome(region_index, BIOME_DESERT);
}

sf::Texture& worldGenerator::getTreeTextureWorld(Biome biome) {
    if(biome.biome_name == "Temperate")
        return this->m_engine->resource.getTexture("panel_tree_warm");
    
    if(biome.biome_name == "Continental" || biome.biome_name == "Tundra")
        return this->m_engine->resource.getTexture("panel_tree_cold");

    if(biome.biome_name == "Tropical" || biome.biome_name == "Mediterranean")
        return this->m_engine->resource.getTexture("panel_tree_tropical");

    else return this->m_engine->resource.getTexture("default");
}

sf::Texture& worldGenerator::getTreeTextureRegion(Biome biome) {
    if(biome.biome_name == "Temperate") {
        int value = std::rand() % 2;

        return (value > 0) 
        ? this->m_engine->resource.getTexture("tile_tree_warm1")
        : this->m_engine->resource.getTexture("tile_tree_warm2");
    }

    else if(biome.biome_name == "Continental" || biome.biome_name == "Tundra")
        return this->m_engine->resource.getTexture("tile_tree_cold1");

    else if(biome.biome_name == "Tropical" || biome.biome_name == "Mediterranean")
        return this->m_engine->resource.getTexture("tile_tree_tropical1");

    else return this->m_engine->resource.getTexture("default");
}

std::string worldGenerator::getTilePixelColour(sf::Vector2i pixel) {
    if(pixel.x < 0 || pixel.y < 0) 
        return "Other";

    if(pixel.x > this->region_settings.tile_size.x - 1 || pixel.y > this->region_settings.tile_size.y - 1) 
        return "Other";

    auto pixel_colour = this->m_engine->resource.getTexture("tile_template_direction").copyToImage().getPixel(pixel.x, pixel.y);

    if(pixel_colour == sf::Color::Red)         return "Red";
    else if(pixel_colour == sf::Color::Green)  return "Green";
    else if(pixel_colour == sf::Color::Blue)   return "Blue";
    else if(pixel_colour == sf::Color::Yellow) return "Yellow";
    else return "Other";
}

// Returns the index of a tile under mouse pointer.
// If index is not found, returns -1.
int worldGenerator::getTileIndex(sf::Vector2f mouse_position, Region& region) {
    // Tiles which intersect with the mouse position.
    std::vector <int> tiles;
    
    int last_index = -1;
    for(int x = 0; x < this->region_settings.size.x; x++) {
        for(int y = 0; y < this->region_settings.size.y; y++) {
            const int index  = y * this->region_settings.size.x + x;
            const Tile& tile = region.map[index];
            sf::FloatRect tile_rectangle  (tile.position, tile.size);
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
        std::abs(mouse_position.x - tile1.position.x + tile1.size.x), 
        std::abs(mouse_position.y - tile1.position.y + tile1.size.y) 
    );    

    auto pos2 = sf::Vector2f(
        std::abs(mouse_position.x - tile2.position.x + tile2.size.x / 2),
        std::abs(mouse_position.y - tile2.position.y + tile2.size.y / 2)
    );

    // If the cursor is closer to the center of one tile.
    if((pos1.x < pos2.x && pos1.y < pos2.y))
        return tiles[0];
    
    else
        return tiles[1];
}

void worldGenerator::regionGenerateRiver(int region_index) {    
    Region& region = this->world_map[region_index];

    if(!region.regiontype.is_river())
        return;

    std::cout << "[World Generation][Region Generation]: Generating river.\n";

    const int river_size = this->region_settings.size.y / 10 / 2;
    RiverDirection direction = region.riverDirection();

    switch(direction) {
        default: {
            std::cout << "[Error][Region Generation]: Unsupported river direction.\n";
            break;
        }
        
        case RiverDirection::RIVER_NONE: {
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

            if(this->world_map[region_index + this->region_settings.size.x].regiontype.is_river()) {
                adjacent_region_direction = 'S';
            }

            if(this->world_map[region_index - this->region_settings.size.x].regiontype.is_river()) {
                adjacent_region_direction = 'N';
            }

            if(adjacent_region_direction == '!') {
                std::cout << "[Error][Region Generator]: There is no adjacent region to river origin.\n";
                return;
            }

            const auto pond_radius = 13;
            const auto pond_centre = sf::Vector2f(this->region_settings.size.x / 2, this->region_settings.size.y / 2);

            // Draw the pond itself.
            for(int x = 0; x < this->region_settings.size.x; x++) {
                for(int y = 0; y < this->region_settings.size.y; y++) {
                    const int index = y * this->region_settings.size.x + x;
                    const bool in_circle = (x - pond_centre.x) * (x - pond_centre.x) + (y - pond_centre.y) * (y - pond_centre.y) < pond_radius * pond_radius; 

                    if(in_circle) {
                        region.map[index].texture = this->m_engine->resource.getTexture("tile_sea");
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
                    noiseSettings settings;
                    settings.size        = sf::Vector2f(this->region_settings.size.y / 2, 1);
                    settings.bias        = 6;
                    settings.octaves     = 10;
                    settings.persistence = 12;
                    settings.multiplier  = 1.25;

                    std::vector <float> disortion;
                    this->generateNoise(settings, disortion);

                    for(int x = this->region_settings.size.x / 2 - river_size; x < this->region_settings.size.x / 2 + river_size; x++) {
                        for(int y = 0; y < this->region_settings.size.y / 2; y++) {
                            const int value = disortion[y] / 0.15f;
                            const int index = (x + value) * this->region_settings.size.x + y;

                            region.map[index].texture = this->m_engine->resource.getTexture("tile_sea");
                            region.map[index].tiletype.set_river();
                        }
                    }

                    break;
                }
                
                case 'S': {
                    noiseSettings settings;
                    settings.size        = sf::Vector2f(this->region_settings.size.y / 2, 1);
                    settings.bias        = 6;
                    settings.octaves     = 10;
                    settings.persistence = 12;
                    settings.multiplier  = 1.25;

                    std::vector <float> disortion;
                    this->generateNoise(settings, disortion);

                    for(int x = this->region_settings.size.x / 2 - river_size; x < this->region_settings.size.x / 2 + river_size; x++) {
                        for(int y = this->region_settings.size.y / 2; y < this->region_settings.size.y; y++) {
                            const int value = disortion[y - this->region_settings.size.y / 2] / 0.15f;
                            const int index = (x + value) * this->region_settings.size.x + y;

                            region.map[index].texture = this->m_engine->resource.getTexture("tile_sea");
                            region.map[index].tiletype.set_river();                        
                        }
                    }

                    break;
                }

                case 'W': {
                    noiseSettings settings;
                    settings.size        = sf::Vector2f(this->region_settings.size.x / 2, 1);
                    settings.bias        = 6;
                    settings.octaves     = 10;
                    settings.persistence = 12;
                    settings.multiplier  = 1.25;
        
                    std::vector <float> disortion;
                    this->generateNoise(settings, disortion);
        
                    for(int x = this->region_settings.size.x / 2; x < this->region_settings.size.x; x++) {
                        for(int y = this->region_settings.size.y / 2 - river_size; y < this->region_settings.size.y / 2 + river_size; y++) {
                            const int value = disortion[x - this->region_settings.size.x / 2] / 0.15f;
                            const int index = x * this->region_settings.size.y + y + value;
                            
                            region.map[index].texture = this->m_engine->resource.getTexture("tile_sea");
                            region.map[index].tiletype.set_river();
                        }
                    }
                    
                    break;
                }

                case 'E': {
                    noiseSettings settings;
                    settings.size        = sf::Vector2f(this->region_settings.size.x / 2, 1);
                    settings.bias        = 6;
                    settings.octaves     = 10;
                    settings.persistence = 12;
                    settings.multiplier  = 1.25;
        
                    std::vector <float> disortion;
                    this->generateNoise(settings, disortion);
        
                    for(int x = 0; x < this->region_settings.size.x / 2; x++) {
                        for(int y = this->region_settings.size.y / 2 - river_size; y < this->region_settings.size.y / 2 + river_size; y++) {
                            const int value = disortion[x] / 0.15f;
                            const int index = x * this->region_settings.size.y + y + value;
                            
                            region.map[index].texture = this->m_engine->resource.getTexture("tile_sea");
                            region.map[index].tiletype.set_river();
                        }
                    }

                    break;
                }
            }

            break;
        }
    
        case RiverDirection::RIVER_HORIZONTAL: {        
            noiseSettings settings;
            settings.size        = sf::Vector2f(this->region_settings.size.x, 1);
            settings.bias        = 6;
            settings.octaves     = 10;
            settings.persistence = 12;
            settings.multiplier  = 1.25;

            std::vector <float> disortion(this->region_settings.size.x);
            this->generateNoise(settings, disortion);

            for(int x = 0; x < this->region_settings.size.x; x++) {
                for(int y = this->region_settings.size.y / 2 - river_size; y < this->region_settings.size.y / 2 + river_size; y++) {
                    const int value = disortion[x] / 0.15f;
                    const int index = x * this->region_settings.size.y + y + value;
                    
                    region.map[index].texture = this->m_engine->resource.getTexture("tile_sea");
                    region.map[index].tiletype.set_river();
                }
            }

            break;
        }

        case RiverDirection::RIVER_VERTICAL: {
            noiseSettings settings;
            settings.size        = sf::Vector2f(this->region_settings.size.x, 1);
            settings.bias        = 6;
            settings.octaves     = 10;
            settings.persistence = 12;
            settings.multiplier  = 1.25;

            std::vector <float> disortion(this->region_settings.size.y);
            this->generateNoise(settings, disortion);

            for(int x = this->region_settings.size.x / 2 - river_size; x < this->region_settings.size.x / 2 + river_size; x++) {
                for(int y = 0; y < this->region_settings.size.y; y++) {
                    const int value = disortion[y] / 0.15f;
                    const int index = (x + value) * this->region_settings.size.x + y;

                    region.map[index].texture = this->m_engine->resource.getTexture("tile_sea");
                    region.map[index].tiletype.set_river();
                }
            }

            break;
        }

        case RiverDirection::RIVER_NORTH_TO_EAST: {
            const sf::Vector2i relative_point = sf::Vector2i(0, 0);
            for(int x = 0; x < this->region_settings.size.x / 2; x++) {
                for(int y = 0; y < this->region_settings.size.y / 2; y++) {
                    const int   index = y * this->region_settings.size.x + x;
                    const float distance_x = (relative_point.x - x) * (relative_point.x - x);
                    const float distance_y = (relative_point.y - y) * (relative_point.y - y);
                    const float distance = sqrt(distance_x + distance_y) / ((this->region_settings.size.x / 4 + this->region_settings.size.y / 4));

                    if(distance > 0.85f && distance < 1.05f) {
                        region.map[index].texture = this->m_engine->resource.getTexture("tile_sea");
                        region.map[index].tiletype.set_river();
                    }
                }
            }

            break;
        }

        case RiverDirection::RIVER_NORTH_TO_WEST: {
            const sf::Vector2i relative_point = sf::Vector2i(0, this->region_settings.size.y);
            for(int x = 0; x < this->region_settings.size.x / 2; x++) {
                for(int y = this->region_settings.size.y / 2; y < this->region_settings.size.y; y++) {
                    const int   index = y * this->region_settings.size.x + x;
                    const float distance_x = (relative_point.x - x) * (relative_point.x - x);
                    const float distance_y = (relative_point.y - y) * (relative_point.y - y);
                    const float distance = sqrt(distance_x + distance_y) / ((this->region_settings.size.x / 4 + this->region_settings.size.y / 4));

                    if(distance > 0.85f && distance < 1.05f) {
                        region.map[index].texture = this->m_engine->resource.getTexture("tile_sea");
                        region.map[index].tiletype.set_river();
                    }
                }
            }

            break;
        }

        case RiverDirection::RIVER_SOUTH_TO_EAST: {
            const sf::Vector2i relative_point = sf::Vector2i(this->region_settings.size.x, 0);
            for(int x = this->region_settings.size.x / 2; x < this->region_settings.size.x; x++) {
                for(int y = this->region_settings.size.y / 2; y >= 0; y--) {
                    const int   index = y * this->region_settings.size.x + x;
                    const float distance_x = (relative_point.x - x) * (relative_point.x - x);
                    const float distance_y = (relative_point.y - y) * (relative_point.y - y);
                    const float distance = sqrt(distance_x + distance_y) / ((this->region_settings.size.x / 4 + this->region_settings.size.y / 4));

                    if(distance > 0.85f && distance < 1.05f) {
                        region.map[index].texture = this->m_engine->resource.getTexture("tile_sea");
                        region.map[index].tiletype.set_river();
                    }
                }
            }
            
            break;
        }

        case RiverDirection::RIVER_SOUTH_TO_WEST: {
            const sf::Vector2i relative_point = sf::Vector2i(this->region_settings.size.x, this->region_settings.size.y);
            for(int x = this->region_settings.size.x - 1; x >= this->region_settings.size.x / 2; x--) {
                for(int y = this->region_settings.size.y - 1; y >= this->region_settings.size.y / 2; y--) {
                    const int   index = y * this->region_settings.size.x + x;
                    const float distance_x = (relative_point.x - x) * (relative_point.x - x);
                    const float distance_y = (relative_point.y - y) * (relative_point.y - y);
                    const float distance = sqrt(distance_x + distance_y) / ((this->region_settings.size.x / 4 + this->region_settings.size.y / 4));

                    if(distance > 0.85f && distance < 1.05f) {
                        region.map[index].texture = this->m_engine->resource.getTexture("tile_sea");
                        region.map[index].tiletype.set_river();
                    }
                }
            }

            break;
        }
    }
}

void worldGenerator::regionGenerateForest(int region_index) {
    Region& region = this->world_map[region_index];

    noiseSettings settings;

    const bool dense = region.regiontype.is_forest();

    if(dense)
        settings = noiseSettings(this->region_settings.size, 8, 12, 4, 1.25f);
    
    else
        settings = noiseSettings(this->region_settings.size, 4, 8, 4, 0.90f);
    
    std::string forest_type = dense ? "dense" : "sparse";
    std::cout << "[World Generation][Region Generation]: Generating a " << forest_type << " forest.\n";

    std::vector <float> noise;
    this->generateNoise(settings, noise);

    for(int i = 0; i < noise.size(); i++) {
        auto& tile = region.map[i];
        if(!tile.tiletype.is_river() && noise[i] > 0.7f) {
            tile.tree = GameObject(tile.position, sf::Vector2f(0, 1.5f * -this->region_settings.tile_size.y), sf::Vector2f(this->region_settings.tile_size.x, 2 * this->region_settings.tile_size.y), &this->getTreeTextureRegion(region.biome));
        }
    }
}

void worldGenerator::regionGenerateHeight(int region_index) {
    Region& region = this->world_map[region_index];

    std::cout << "[World Generation][Region Generation]: Generating flatlands.\n";

    // Height modifier for fancier terrain.
    const float height_modifier = 1.2f;

    noiseSettings height_settings(this->region_settings.size, 16, 16, 4, height_modifier);
    std::vector <float> height;
    this->generateNoise(height_settings, height);

    const RiverDirection river_direction = region._direction;

    for(int i = 0; i < region.map.size(); i++) {
        region.map[i]._marked = false;
    }

    // How many tiles from the river should be smoothed to create a valley-like look.
    const int terrain_slope = (this->region_settings.size.x + this->region_settings.size.y) / 2 / 10;
    const auto smaller_slope = [&](int slope) -> float {
        return std::floor(std::abs(std::sqrt(slope) - 1));
    };

    for(int y = 0; y < this->region_settings.size.y; y++) {
        for(int x = 0; x < this->region_settings.size.x; x++) {
            const int index = y * this->region_settings.size.x + x;

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
                    if(index + 1 < this->region_settings.size.x * this->region_settings.size.y) {
                        if(!region.map[index + 1].tiletype.is_river()) {
                            for(int slope_right = 0; slope_right < terrain_slope; slope_right++) {
                                const int index_right = index + slope_right;
    
                                if(index_right < this->region_settings.size.x * this->region_settings.size.y) {
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
                    if(index - this->region_settings.size.x >= 0) {
                        if(!region.map[index - this->region_settings.size.x].tiletype.is_river()) {
                            for(int slope_top = 0; slope_top < terrain_slope; slope_top++) {
                                const int index_top = index - slope_top * this->region_settings.size.x;

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
                    if(index + this->region_settings.size.x < this->region_settings.size.x * this->region_settings.size.y) {
                        if(!region.map[index + this->region_settings.size.x].tiletype.is_river()) {
                            for(int slope_bottom = 0; slope_bottom < terrain_slope; slope_bottom++) {
                                const int index_bottom = index + slope_bottom * this->region_settings.size.x;

                                if(index_bottom < this->region_settings.size.x * this->region_settings.size.y) {
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
                    if(index - 1 >= 0 && index - 1 >= y * this->region_settings.size.x) {
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
                    if(index + 1 < this->region_settings.size.x * this->region_settings.size.y && index + 1 < (y + 1) * this->region_settings.size.x) {
                        if(!region.map[index + 1].tiletype.is_river()) {
                            for(int slope_right = 0; slope_right < terrain_slope; slope_right++) {
                                const int index_right = index + slope_right;

                                if(index_right < this->region_settings.size.x * this->region_settings.size.y) {
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

                    // For the southern coast.
                    if(index - this->region_settings.size.x >= 0) {
                        if(!region.map[index - this->region_settings.size.x].tiletype.is_river()) {
                            for(int slope_left = 0; slope_left < terrain_slope; slope_left++) {
                                const int index_left = index - slope_left * this->region_settings.size.x;
                    
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
                    if(index + this->region_settings.size.x < this->region_settings.size.x * this->region_settings.size.y) {
                        if(!region.map[index + this->region_settings.size.x].tiletype.is_river()) {
                            for(int slope_right = 0; slope_right < terrain_slope; slope_right++) {
                                const int index_right = index + slope_right * this->region_settings.size.x;
    
                                if(index_right < this->region_settings.size.x * this->region_settings.size.y) {
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
                const int elevation_top  = (index - this->region_settings.size.x >= 0) ? std::ceil(region.map[index - this->region_settings.size.x].elevation * height_modifier) : 0;
                const int elevation_left = (index - 1 >= 0)                            ? std::ceil(region.map[index - 1].elevation * height_modifier) : 0;

                int elevation = height[index] / 0.25f;
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
        region.map[i].position = region.map[i].position + sf::Vector2f(0, -region.map[i].elevation * this->region_settings.tile_size.y / 2);
}
