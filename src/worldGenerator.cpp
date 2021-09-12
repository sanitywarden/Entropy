#include "worldGenerator.hpp"

using namespace iso;

worldGenerator::worldGenerator() {

}

worldGenerator::worldGenerator(WorldSettings& world_settings, RegionSettings& region_settings, entropy::Entropy* engine) {
    this->m_world_settings  = world_settings;
    this->m_region_settings = region_settings;
    this->m_engine          = engine;
    this->m_log_prefix      = "[World Generation]";

    auto world_size = this->m_world_settings.size.x * this->m_world_settings.size.y;

    this->m_noise.resize(world_size);
    this->m_gradient.resize(world_size);
    this->world_map.resize(world_size);
    this->region_map.resize(world_size);
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

    for(int i = 0; i < this->world_map.size(); i++) {
        this->world_map[i] = iso::Panel();
    }

    for(int x = 0; x < this->m_world_settings.size.x; x++) {
        for(int y = 0; y < this->m_world_settings.size.y; y++) {                
            float noise    = this->m_noise[y * this->m_world_settings.size.x + x]; 
            float gradient = this->m_gradient[y * this->m_world_settings.size.x + x];
            float value = noise - gradient;

            this->m_panel.is_arctic = false;
            this->m_panel.is_coast  = false;

            if(value > 0.3f) {
                this->m_panel.is_terrain = true;
                panel_quantity++;
            }

            else {
                this->m_panel.is_terrain = false;
            }                                      

            if(value < 0.0f) value = 0.0f;
            if(value > 1.0f) value = 1.0f;

            int index = y * this->m_world_settings.size.x + x;

            this->m_panel.region.height = value;
            this->m_panel.panel_position = sf::Vector2f(x * this->m_world_settings.panel_size.x, y * this->m_world_settings.panel_size.y); 
            this->m_panel.panel_size = this->m_world_settings.panel_size;

            this->world_map[index] = this->m_panel;
        }
    }

    std::cout << this->m_log_prefix << ": Generating cold poles.\n";
    
    this->generatePoles();

    // Find and mark coast tiles.
    for(int x = 0; x < this->m_world_settings.size.x; x++) {
        for(int y = 0; y < this->m_world_settings.size.y; y++) {
            int index_current = y * this->m_world_settings.size.x + x;
            
            if(this->world_map[index_current].is_terrain) {
                int index_up    = index_current - this->m_world_settings.size.x;             
                int index_down  = index_current + this->m_world_settings.size.x;
                int index_left  = index_current - 1;
                int index_right = index_current + 1;

                if(index_up > 0) {
                    if(!this->world_map[index_up].is_terrain) 
                        this->world_map[index_current].is_coast = true;
                }

                if(index_left > 0) {
                    if(!this->world_map[index_left].is_terrain) 
                        this->world_map[index_current].is_coast = true;
                }

                if(index_down < this->m_world_settings.size.x * this->m_world_settings.size.y) {
                    if(!this->world_map[index_down].is_terrain) 
                        this->world_map[index_current].is_coast = true;
                }

                if(index_right < this->m_world_settings.size.x * this->m_world_settings.size.y) {
                    if(!this->world_map[index_right].is_terrain) 
                        this->world_map[index_current].is_coast = true;
                }
            }
        }
    }

    std::cout << this->m_log_prefix << ": Generating river(s).\n";
    
    this->generateRivers();
    this->generateLatititude();
    this->generateTemperature();
    this->generateMoistureMap();

    this->assignBiome();

    std::cout << this->m_log_prefix << ": World generated.\n";
    std::cout << this->m_log_prefix << ": Terrain is " << panel_quantity  << " panels out of total " << this->m_world_settings.size.x * this->m_world_settings.size.y << " panels" << " (" << int(float(panel_quantity / float(this->m_world_settings.size.x * this->m_world_settings.size.y)) * 100) << "%)." << "\n";
    std::cout << this->m_log_prefix << ": " << this->m_world_settings.size.x * this->m_world_settings.size.y << " panels generated in " << clock.getElapsedTime().asSeconds() << " seconds.\n";
}

void worldGenerator::generateNoiseMap() {
    std::vector <float> input(this->m_world_settings.size.x * this->m_world_settings.size.y);

    for(int i = 0; i < this->m_world_settings.size.x * this->m_world_settings.size.y; i++) {
        input[i] = (float)rand() / (float)RAND_MAX;    
    }

    float biggest_noise_recorded = 1.0f;

    for(int x = 0; x < this->m_world_settings.size.x; x++) {
        for(int y = 0; y < this->m_world_settings.size.y; y++) {
            if(x < this->m_world_settings.margin.x || x > (this->m_world_settings.size.x - this->m_world_settings.margin.x) || y < this->m_world_settings.margin.y || y > (this->m_world_settings.size.y - this->m_world_settings.margin.y)) {
                this->m_noise[y * this->m_world_settings.size.x + x] = 0.0f;
                continue;
            }
            
            float noise = 0.0f;
            float scale = 1.0f;
            float scale_acc = 0.0f;

            for(unsigned int o = 0; o < this->m_world_settings.noise_octaves; o++) {
                int sampleX1 = (x / this->m_world_settings.noise_persistence) * this->m_world_settings.noise_persistence;
                int sampleY1 = (y / this->m_world_settings.noise_persistence) * this->m_world_settings.noise_persistence;

                int sampleX2 = (sampleX1 + this->m_world_settings.noise_persistence) % this->m_world_settings.size.x;					
                int sampleY2 = (sampleY1 + this->m_world_settings.noise_persistence) % this->m_world_settings.size.x;

                float blendX = (float)(x - sampleX1) / (float)this->m_world_settings.noise_persistence;
                float blendY = (float)(y - sampleY1) / (float)this->m_world_settings.noise_persistence;

                float sampleT = (1.0f - blendX) * input[sampleY1 * this->m_world_settings.size.x + sampleX1] + blendX * input[sampleY1 * this->m_world_settings.size.x + sampleX2];
                float sampleB = (1.0f - blendX) * input[sampleY2 * this->m_world_settings.size.x + sampleX1] + blendX * input[sampleY2 * this->m_world_settings.size.x + sampleX2];

                scale_acc += scale;
                noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
                scale = float(scale / this->m_world_settings.noise_bias);
            }
            
            float noise_value = (noise / scale_acc) * this->m_world_settings.multiplier_noise;
            
            if(noise_value > biggest_noise_recorded) biggest_noise_recorded = noise_value;

            noise_value *= biggest_noise_recorded;
            if(noise_value > 1.0f) noise_value = 1.0f;

            this->m_noise[y * this->m_world_settings.size.x + x] = noise_value;
        }
    }
}

void worldGenerator::generateCircularGradient() {
    sf::Vector2i centre = sf::Vector2i(
        this->m_world_settings.size.x / 2,
        this->m_world_settings.size.y / 2
    );

    for(int x = 0; x < this->m_world_settings.size.x; x++) {
        for(int y = 0; y < this->m_world_settings.size.y; y++) {
            if(x < this->m_world_settings.margin.x || x > (this->m_world_settings.size.x - this->m_world_settings.margin.x) || y < this->m_world_settings.margin.y || y > (this->m_world_settings.size.y - this->m_world_settings.margin.y)) {
                this->m_gradient[y * this->m_world_settings.size.x + x] = 0.0f;
                continue;
            }

            float distance_x = (centre.x - x) * (centre.x - x);
            float distance_y = (centre.y - y) * (centre.y - y);

            // Both width and height of the map have a impact on distance.
            float distance = (sqrt(distance_x + distance_y) / (0.5f * (this->m_world_settings.size.x + this->m_world_settings.size.y)));

            if(distance > 1.0f) distance = 1.0f;

            // Distance is bigger when further away from the centre of the gradient.
            this->m_gradient[y * this->m_world_settings.size.x + x] = distance * this->m_world_settings.multiplier_gradient;
        }
    }
}

void worldGenerator::generatePoles() {
    const float chance   = 1.0f;
    const float modifier = 0.6f;

    for(int x = 0; x < this->m_world_settings.size.x; x++) {
        float pole_chance   = chance;
        float pole_modifier = modifier;

        for(unsigned int y = 0; y < this->m_world_settings.pole_size; y++) {
            float random_number1 = (float)rand();
            float random_number2 = (float)RAND_MAX * pole_chance;

            if(random_number1 < random_number2) {
                this->world_map[y * this->m_world_settings.size.x + x].is_terrain = true;
                this->world_map[y * this->m_world_settings.size.x + x].is_arctic = true;
            }   
        
            pole_chance *= pole_modifier;
        }
    }

    for(int x = 0; x < this->m_world_settings.size.x; x++) { 
        float pole_chance   = chance;
        float pole_modifier = modifier;

        for(unsigned int y = this->m_world_settings.size.y - 1; y > this->m_world_settings.size.y - 1 - this->m_world_settings.pole_size; y--) {
            float random_number1 = (float)rand();
            float random_number2 = (float)RAND_MAX * pole_chance;

            if(random_number1 < random_number2) {
                this->world_map[y * this->m_world_settings.size.x + x].is_terrain = true;
                this->world_map[y * this->m_world_settings.size.x + x].is_arctic = true;
            }

            pole_chance *= pole_modifier;
        }

        pole_chance   = chance;
        pole_modifier = modifier;
    }
}

void worldGenerator::generateRivers() {
    // Number of rivers is a maximum amount of rivers.
    // It does not mean that there will be the specified amount.

    // Storage for indexes of already created rivers.
    std::vector <int> river_origin_index(this->m_world_settings.river_quantity);
    
    for(unsigned int river_number = 0; river_number < this->m_world_settings.river_quantity; river_number++) {
        int possible_river_origin_index = rand() % this->world_map.size();
        bool place_found = false;

        while(!place_found) {
            int index = rand() % this->world_map.size();
            
            if(this->world_map[index].is_terrain && !this->world_map[index].is_arctic && !this->world_map[index].is_coast) {
                possible_river_origin_index = index;
                place_found = true;
            }
        }

        // Area (Panels) that should be scanned in each geographical direction from the possible river origin position. 
        const int area_around_origin = (this->m_world_settings.size.x + this->m_world_settings.size.y) / 2 / 15;

        unsigned int rivers_verified = 0;
        
        // Scan the provided area around the possible origin of the river to check if it is suitable.
        // The area is suitable if there are no rivers nearby.             
        for(size_t scanned_river = 0; scanned_river < river_origin_index.size(); scanned_river++) {
            int panels_verified = 0;
            int panels_occupied = 0;
            
            for(int x = 0 - area_around_origin; x <= 0 + area_around_origin; x++) {
                for(int y = 0 - area_around_origin; y <= 0 + area_around_origin; y++) {
                    int index = possible_river_origin_index + (x * 1 + y * this->m_world_settings.size.y);

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
        if(rivers_verified == this->m_world_settings.river_quantity) {
            river_index = possible_river_origin_index;
            river_origin_index[river_number] = possible_river_origin_index;
        }

        // Suitable position for a new river was not found; Ignore the rest of the algorithm and move on to the next river.
        if(river_index == -1) {    
            continue;
        }

        // Create a path to the sea.
        bool coast_found = false;
        int river_index_start   = river_index;
        int river_index_current = river_index;
        int last_move_value     = 0;

        // Create a path to the sea by following the lowest terrain until coast if found.
        while(true) {                
            this->world_map[river_index_current].marked = true;

            const int index_up    = river_index_current - this->m_world_settings.size.x;
            const int index_down  = river_index_current + this->m_world_settings.size.x;
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
                if(!this->world_map[values[i]].marked && this->world_map[current_move_direction].region.height > this->world_map[values[i]].region.height) current_move_direction = values[i];
            }

            const int current_move_value = current_move_direction - river_index_current;

            std::string text_direction = "START"; 

            if(river_index_current != river_index_start) {
                if(current_move_direction == index_up) {
                    // UP AND RIGHT
                    if(current_move_value + last_move_value == -this->m_world_settings.size.x + 1) {
                        text_direction = "UP AND RIGHT";
                    }

                    // UP AND LEFT
                    if(current_move_value + last_move_value == -this->m_world_settings.size.x - 1) {
                        text_direction = "UP AND LEFT";
                    }

                    // UP
                    if(current_move_value == -this->m_world_settings.size.x && last_move_value == -this->m_world_settings.size.x) {
                        text_direction = "UP";
                    }

                }

                else if(current_move_direction == index_down) {
                    // DOWN AND RIGHT
                    if(current_move_value + last_move_value == this->m_world_settings.size.x + 1) {
                        text_direction = "DOWN AND RIGHT";
                    }

                    // DOWN AND LEFT
                    if(current_move_value + last_move_value == this->m_world_settings.size.x - 1) {
                        text_direction = "DOWN AND LEFT";
                    }

                    // DOWN
                    if(current_move_value == this->m_world_settings.size.x && last_move_value == this->m_world_settings.size.x) {
                        text_direction = "DOWN";
                    }
                }

                else if(current_move_direction == index_left) {
                    // LEFT
                    if(current_move_value + last_move_value == -1 || -2) {
                        text_direction = "LEFT";
                    }

                    // LEFT AND UP
                    if(current_move_value + last_move_value == -1 - this->m_world_settings.size.x) {
                        text_direction = "LEFT AND UP";
                    }

                    // LEFT AND DOWN
                    if(current_move_value + last_move_value == -1 + this->m_world_settings.size.x) {
                        text_direction = "LEFT AND DOWN";
                    }

                }

                else if(current_move_direction == index_right) {
                    // RIGHT
                    if(current_move_value + last_move_value == 1 || 2) {
                        text_direction = "RIGHT";
                    }

                    // RIGHT AND UP
                    if(current_move_value + last_move_value == 1 - this->m_world_settings.size.x) {
                        text_direction = "RIGHT AND UP";
                    }

                    // RIGHT AND DOWN
                    if(current_move_value + last_move_value == 1 + this->m_world_settings.size.x) {
                        text_direction = "RIGHT AND DOWN";
                    }
                }
            }

            this->world_map[river_index_current].is_river = true;

            if(text_direction == "START" && river_index_start == river_index_current) this->world_map[river_index_current].feature.river.texture = &this->m_engine->resource.getTexture("panel_sea");
            
            else if(text_direction == "DOWN") {
                this->world_map[river_index_current].feature.river.texture = &this->m_engine->resource.getTexture("panel_river_vertical");
            }

            else if(text_direction == "DOWN AND LEFT") {
                this->world_map[river_index_current].feature.river.texture = &this->m_engine->resource.getTexture("panel_river_corner_tl");    
            } 
            
            else if(text_direction == "DOWN AND RIGHT") {
                this->world_map[river_index_current].feature.river.texture = &this->m_engine->resource.getTexture("panel_river_corner_tr");
            }
            
            else if(text_direction == "UP") {
                this->world_map[river_index_current].feature.river.texture = &this->m_engine->resource.getTexture("panel_river_vertical");
            }
            
            else if(text_direction == "RIGHT AND UP") {
                this->world_map[river_index_current].feature.river.texture = &this->m_engine->resource.getTexture("panel_river_corner_tl");
            }
            
            else if(text_direction == "RIGHT AND DOWN") {
                this->world_map[river_index_current].feature.river.texture = &this->m_engine->resource.getTexture("panel_river_corner_bl");
            }
            
            else if(text_direction == "LEFT") {
                this->world_map[river_index_current].feature.river.texture = &this->m_engine->resource.getTexture("panel_river_horizontal");
            }
            
            else if(text_direction == "LEFT AND UP") {
                this->world_map[river_index_current].feature.river.texture = &this->m_engine->resource.getTexture("panel_river_corner_tr");
            }
            
            else if(text_direction == "LEFT AND DOWN") {
                this->world_map[river_index_current].feature.river.texture = &this->m_engine->resource.getTexture("panel_river_corner_br");
            }
            
            else if(text_direction == "RIGHT") {
                this->world_map[river_index_current].feature.river.texture = &this->m_engine->resource.getTexture("panel_river_horizontal");
            }
            
            else if(text_direction == "UP AND LEFT") {
                this->world_map[river_index_current].feature.river.texture = &this->m_engine->resource.getTexture("panel_river_corner_bl");
            }
            
            else if(text_direction == "UP AND RIGHT") {
                this->world_map[river_index_current].feature.river.texture = &this->m_engine->resource.getTexture("panel_river_corner_br");
            }

            last_move_value     = current_move_value;
            river_index_current = current_move_direction;

            if(coast_found) break;

            if(this->world_map[river_index_current].is_coast) {
                coast_found = true;
            }
        }
    }
}

void worldGenerator::generateLatititude() {
    // There definately is code that does not require the if statement, but it is how it is.

    for(int x = 0; x < this->m_world_settings.size.x; x++) {
        for(int y = 0; y < this->m_world_settings.size.y; y++) {            
            // Latitude for the upper half.
            if(y < this->m_world_settings.size.y / 2)
                this->world_map[y * this->m_world_settings.size.x + x].region.latitude = float(y + 1) / float(this->m_world_settings.size.y / 2);

            // Latitude for the bottom half.
            else {
                int i = y - this->m_world_settings.size.y / 2;
                int reversed_height = y - 2 * i;
                this->world_map[y * this->m_world_settings.size.x + x].region.latitude = float(reversed_height) / float(this->m_world_settings.size.y / 2);
            }
        }
    }
}

void worldGenerator::generateTemperature() {
    for(int x = 0; x < this->m_world_settings.size.x; x++) {
        for(int y = 0; y < this->m_world_settings.size.y; y++) {            
            const int panel_index = y * this->m_world_settings.size.x + x;

            auto& panel = this->world_map[panel_index];

            // Panel temperature zone classification.
            const bool PANEL_ARCTIC = (y < this->m_world_settings.pole_size || y > this->m_world_settings.size.y - this->m_world_settings.pole_size - 1) ? true : false;
            const bool PANEL_NEAR_POLE = ((y < this->m_world_settings.pole_size + this->m_world_settings.margin.y + 0.1f * (float)this->m_world_settings.size.y && y >= this->m_world_settings.pole_size) || (y >= this->m_world_settings.size.y - (this->m_world_settings.pole_size + this->m_world_settings.margin.y + 0.1f * (float)this->m_world_settings.size.y) && y < this->m_world_settings.size.y - this->m_world_settings.pole_size)) ? true : false;
            const bool PANEL_NEAR_EQUATOR = (y > this->m_world_settings.size.y / 2 - 1 - this->m_world_settings.margin.y && y <= this->m_world_settings.size.y / 2 + 1 + this->m_world_settings.margin.y) ? true : false;

            if(PANEL_ARCTIC) {
                panel.region.temperature = 0.05f + panel.region.latitude * panel.region.latitude + 1.0f / (float)(rand() % 10 + 5);
            }

            else if(PANEL_NEAR_POLE) {
                panel.region.temperature = 0.1f + panel.region.latitude * panel.region.latitude + 1.0f / (float)(rand() % 10 + 5);
            }
            
            else if(PANEL_NEAR_EQUATOR) {
                panel.region.temperature = -0.1f + panel.region.latitude * panel.region.latitude + 1.0f / (float)(rand() % 10 + 5);
            }

            else {
                panel.region.temperature = 0.4f + ((float)rand() / (float)RAND_MAX / 2) * ((1.0f - panel.region.height) * panel.region.latitude);
            }

            if(panel.region.temperature > 1.0f) panel.region.temperature = 1.0f;
        }
    }
}

void worldGenerator::generateMoistureMap() {
    std::vector <float> input(this->m_world_settings.size.x * this->m_world_settings.size.y);
    for(int i = 0; i < this->m_world_settings.size.x * this->m_world_settings.size.y; i++) {
        input[i] = (float)rand() / (float)RAND_MAX;
    }
    
    float biggest_noise_recorded = 1.0f;

    for(int x = 0; x < this->m_world_settings.size.x; x++) {
        for(int y = 0; y < this->m_world_settings.size.y; y++) {
            int index = y * this->m_world_settings.size.x + x;

            if(!this->world_map[index].is_terrain)
                this->world_map[index].region.moisture = 1.0f;

            else if(this->world_map[index].is_arctic)
                this->world_map[index].region.moisture = 0.0f;

            else if(this->world_map[index].is_terrain && !this->world_map[index].is_arctic) {
                float noise = 0.0f;
                float scale = 1.0f;
                float scale_acc = 0.0f;

                for(unsigned int o = 0; o < this->m_world_settings.moisture_octaves; o++) {
                    int sampleX1 = (x / this->m_world_settings.moisture_persistence) * this->m_world_settings.moisture_persistence;
                    int sampleY1 = (y / this->m_world_settings.moisture_persistence) * this->m_world_settings.moisture_persistence;

                    int sampleX2 = (sampleX1 + this->m_world_settings.moisture_persistence) % this->m_world_settings.size.x;					
                    int sampleY2 = (sampleY1 + this->m_world_settings.moisture_persistence) % this->m_world_settings.size.x;

                    float blendX = (float)(x - sampleX1) / (float)this->m_world_settings.moisture_persistence;
                    float blendY = (float)(y - sampleY1) / (float)this->m_world_settings.moisture_persistence;

                    float sampleT = (1.0f - blendX) * input[sampleY1 * this->m_world_settings.size.x + sampleX1] + blendX * input[sampleY1 * this->m_world_settings.size.x + sampleX2];
                    float sampleB = (1.0f - blendX) * input[sampleY2 * this->m_world_settings.size.x + sampleX1] + blendX * input[sampleY2 * this->m_world_settings.size.x + sampleX2];

                    scale_acc += scale;
                    noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
                    scale = float(scale / this->m_world_settings.moisture_bias);
                }
                
                float noise_value = (noise / scale_acc) * this->m_world_settings.multiplier_moisture;
                
                if(noise_value > biggest_noise_recorded) biggest_noise_recorded = noise_value;

                noise_value *= biggest_noise_recorded;
            
                if(this->world_map[index].region.height > 0.85f) {
                    noise_value *= 0.1f + this->world_map[index].region.latitude * this->world_map[index].region.latitude * (1.0f - this->world_map[index].region.height);
                }

                if(this->world_map[index].region.latitude < 0.55f) {
                    noise_value *= this->world_map[index].region.latitude * noise_value;
                    noise_value += 0.2f + (0.2f * noise_value);
                }

                else noise_value += 0.2f * this->world_map[index].region.moisture;

                if(noise_value > 1.0f) noise_value = 1.0f;

                this->world_map[y * this->m_world_settings.size.x + x].region.moisture = noise_value;
            }
        }
    }
}

void worldGenerator::assignBiome() {
    for(int i = 0; i < this->m_world_settings.size.x * this->m_world_settings.size.y; i++) {
        auto& panel = this->world_map[i];

        if(!panel.is_terrain)    panel.panel_texture = this->m_engine->resource.getTexture("panel_ocean");
        else if(panel.is_arctic) panel.panel_texture = this->m_engine->resource.getTexture("panel_arctic");
    
        else if(panel.region.temperature < 0.45f && panel.region.latitude < 0.5f) {    
            if(panel.region.moisture > 0.23f)       panel.panel_texture = this->m_engine->resource.getTexture("panel_grass_cold");
            else                                    panel.panel_texture = this->m_engine->resource.getTexture("panel_tundra");
        }

        else if(panel.region.temperature < 0.55f && panel.region.latitude < 0.55f) {
            int tiletype = rand() % 10;

            if(tiletype > 5)      panel.panel_texture = this->m_engine->resource.getTexture("panel_grass_warm");
            else if(tiletype > 2) panel.panel_texture = this->m_engine->resource.getTexture("panel_grass_cold");
            else                  panel.panel_texture = this->m_engine->resource.getTexture("panel_tundra");
        }

        else if(panel.region.temperature < 0.7f) {
            panel.panel_texture = this->m_engine->resource.getTexture("panel_grass_warm"); 
        }

        else {
            if(panel.region.moisture > 0.51f)      panel.panel_texture = this->m_engine->resource.getTexture("panel_grass_tropical");
            else if(panel.region.moisture > 0.13f) panel.panel_texture = this->m_engine->resource.getTexture("panel_grass_subtropical");    
            else                                   panel.panel_texture = this->m_engine->resource.getTexture("panel_grass_warm");
        } 
    }
}

void worldGenerator::generateRegion(Region& region_data, int index) {
    for(int y = 0; y < region_data.map_size.y; y++) {
        for(int x = 0; x < region_data.map_size.x; x++) {
            auto tile_position = sf::Vector2i(
                (this->m_region_settings.tile_offset.x * this->m_region_settings.tile_size.x) + (x - y) * this->m_region_settings.tile_size.x / 2,
                (this->m_region_settings.tile_offset.y * this->m_region_settings.tile_size.y) + (x + y) * this->m_region_settings.tile_size.y / 2
            );

            auto world_position = sf::Vector2f(
                this->m_engine->window.getWindow()->mapPixelToCoords(tile_position).x,
                this->m_engine->window.getWindow()->mapPixelToCoords(tile_position).y
            );

            this->m_tile.tile_size     = this->m_region_settings.tile_size;
            this->m_tile.tile_position = tile_position;
            this->m_tile.tile_texture  = this->m_engine->resource.getTexture("tile_grass_warm");

            region_data.map.push_back(this->m_tile);
        }
    }

    this->region_map[index] = region_data;
}