#include "worldGenerator.hpp"

using namespace iso;

worldGenerator::worldGenerator() {

}

worldGenerator::worldGenerator(WorldSettings& settings, entropy::Entropy* engine) {
    this->m_world_settings = settings;
    this->m_engine = engine;
    this->m_log_prefix = "[World Generation]";

    auto world_size = this->m_world_settings.size.x * this->m_world_settings.size.y;

    this->m_noise.resize(world_size);
    this->m_gradient.resize(world_size);
    this->world_map.resize(world_size);

    this->generateWorld();
}

worldGenerator::~worldGenerator() {

}

void worldGenerator::generateWorld() {
    std::cout << this->m_log_prefix << ": Generating world.\n";
    std::cout << this->m_log_prefix << ": World size: " << this->m_world_settings.size.x << "x" << this->m_world_settings.size.y << ".\n";
    
    sf::Clock clock;
    unsigned int panel_quantity = 0;

    std::cout << this->m_log_prefix << ": Generating noise.\n";

    this->generatePerlinNoise();
    this->generateCircularGradient();

    std::cout << this->m_log_prefix << ": Generating terrain.\n";

    for(int x = 0; x < this->m_world_settings.size.x; x++) {
        for(int y = 0; y < this->m_world_settings.size.y; y++) {                
            float noise    = this->m_noise[y * this->m_world_settings.size.x + x]; 
            float gradient = this->m_gradient[y * this->m_world_settings.size.x + x];
            float value = noise - gradient;

            this->m_panel.is_arctic = false;
            this->m_panel.is_coast  = false;

            if(value > 0.4f) {
                this->m_panel.is_terrain = true;

                panel_quantity++;
            }

            else {
                this->m_panel.is_terrain = false;
            }                                      

            if(value < 0.0f) value = 0.0f;
            if(value > 1.0f) value = 1.0f;

            this->m_panel.noise_value = value;
            this->m_panel.panel_position = sf::Vector2f(x * this->m_world_settings.panel_size.x, y * this->m_world_settings.panel_size.y); 
            this->m_panel.panel_size = this->m_world_settings.panel_size;

            int index = y * this->m_world_settings.size.x + x;

            this->world_map[index] = this->m_panel;
        }
    }

    for(size_t i = 0; i < this->world_map.size(); i++) {
        if(this->world_map[i].is_terrain && !this->world_map[i].is_arctic) {
            this->world_map[i].panel_texture = this->m_engine->resource.getTexture("panel_grass_warm");
        }

        if(!this->world_map[i].is_terrain && !this->world_map[i].is_arctic) {
            this->world_map[i].panel_texture = this->m_engine->resource.getTexture("panel_ocean");
        }
    }

    std::cout << this->m_log_prefix << ": Generating cold poles.\n";
    
    this->generatePoles();

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

    std::cout << this->m_log_prefix << ": World generated.\n";
    std::cout << this->m_log_prefix << ": Terrain is " << panel_quantity  << " panels out of total " << this->m_world_settings.size.x * this->m_world_settings.size.y << " panels" << " (" << int(float(panel_quantity / float(this->m_world_settings.size.x * this->m_world_settings.size.y)) * 100) << "%)." << "\n";
    std::cout << this->m_log_prefix << ": " << this->m_world_settings.size.x * this->m_world_settings.size.y << " panels generated in " << clock.getElapsedTime().asSeconds() << " seconds.\n";
}

void worldGenerator::generatePerlinNoise() {
    std::vector <float> input(this->m_world_settings.size.x * this->m_world_settings.size.y);

    for(int i = 0; i < this->m_world_settings.size.x * this->m_world_settings.size.y; i++) {
        input[i] = (float)rand() / (float)RAND_MAX;    
    }

    for(int x = 0; x < this->m_world_settings.size.x; x++) {
        for(int y = 0; y < this->m_world_settings.size.y; y++) {
            if(x < this->m_world_settings.margin.x || x > (this->m_world_settings.size.x - this->m_world_settings.margin.x) || y < this->m_world_settings.margin.y || y > (this->m_world_settings.size.y - this->m_world_settings.margin.y)) {
                this->m_noise[y * this->m_world_settings.size.x + x] = 0.0f;
                continue;
            }
            
            const float multiplier = 2.0f;
            float noise = 0.0f;
            float scale = 1.0f;
            float scale_acc = 0.0f;

            for(unsigned int o = 0; o < this->m_world_settings.octaves; o++) {
                int sampleX1 = (x / this->m_world_settings.persistence) * this->m_world_settings.persistence;
                int sampleY1 = (y / this->m_world_settings.persistence) * this->m_world_settings.persistence;

                int sampleX2 = (sampleX1 + this->m_world_settings.persistence) % this->m_world_settings.size.x;					
                int sampleY2 = (sampleY1 + this->m_world_settings.persistence) % this->m_world_settings.size.x;

                float blendX = (float)(x - sampleX1) / (float)this->m_world_settings.persistence;
                float blendY = (float)(y - sampleY1) / (float)this->m_world_settings.persistence;

                float sampleT = (1.0f - blendX) * input[sampleY1 * this->m_world_settings.size.x + sampleX1] + blendX * input[sampleY1 * this->m_world_settings.size.x + sampleX2];
                float sampleB = (1.0f - blendX) * input[sampleY2 * this->m_world_settings.size.x + sampleX1] + blendX * input[sampleY2 * this->m_world_settings.size.x + sampleX2];

                scale_acc += scale;
                noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
                scale = float(scale / this->m_world_settings.bias);
            }
            
            float noise_value = (noise / scale_acc) * multiplier;
            
            if(noise_value > 1.0f) noise_value = 1.0f;
            
            this->m_noise[y * this->m_world_settings.size.x + x] = noise_value;
        }
    }
}

float worldGenerator::generateNoise(int& x, int& y) {
    float v1 = (float)x / (float)y;
    float v2 = (float)rand() / (float)RAND_MAX;
    return (v2 * v1) / 2;
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
            float distance = sqrt(distance_x + distance_y) / (0.5f * (this->m_world_settings.size.x + this->m_world_settings.size.y));

            distance *= 2;

            if(distance > 1.0f) distance = 1.0f;

            // Distance is bigger when further away from the centre of the gradient.
            this->m_gradient[y * this->m_world_settings.size.x + x] = distance;
        }
    }
}

void worldGenerator::generatePoles() {
    const float chance   = 1.0f;
    const float modifier = 0.6f;

    const int pole_size = this->m_world_settings.size.y / 10;

    for(int x = 0; x < this->m_world_settings.size.x; x++) {
        float pole_chance   = chance;
        float pole_modifier = modifier;

        for(unsigned int y = 0; y < pole_size; y++) {
            float random_number1 = (float)rand();
            float random_number2 = (float)RAND_MAX * pole_chance;

            if(random_number1 < random_number2) {
                this->world_map[y * this->m_world_settings.size.x + x].is_terrain = true;
                this->world_map[y * this->m_world_settings.size.x + x].is_arctic = true;
                this->world_map[y * this->m_world_settings.size.x + x].panel_texture = this->m_engine->resource.getTexture("panel_arctic");
            }   
        
            pole_chance *= pole_modifier;
        }
    }

    for(int x = 0; x < this->m_world_settings.size.x; x++) { 
        float pole_chance   = chance;
        float pole_modifier = modifier;

        for(unsigned int y = this->m_world_settings.size.y - 1; y > this->m_world_settings.size.y - 1 - pole_size; y--) {
            float random_number1 = (float)rand();
            float random_number2 = (float)RAND_MAX * pole_chance;

            if(random_number1 < random_number2) {
                this->world_map[y * this->m_world_settings.size.x + x].is_terrain = true;
                this->world_map[y * this->m_world_settings.size.x + x].is_arctic = true;
                this->world_map[y * this->m_world_settings.size.x + x].panel_texture = this->m_engine->resource.getTexture("panel_arctic");
            }

            pole_chance *= pole_modifier;
        }

        pole_chance   = chance;
        pole_modifier = modifier;
    }
}

void worldGenerator::generateRivers() {
    // This number does not mean that there will be so many rivers.
    // This servers as the maximum possible number of rivers.
    const int river_quantity = (this->m_world_settings.size.x + this->m_world_settings.size.y) / 10 / 2;
    
    // Storage for indexes of already created rivers.
    std::vector <int> river_origin_index(river_quantity);
    
    for(unsigned int river_number = 0; river_number < river_quantity; river_number++) {
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
        const int area_around_origin = (this->m_world_settings.size.x + this->m_world_settings.size.y) / 2 / 10;

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
        if(rivers_verified == river_quantity) {
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
                if(!this->world_map[values[i]].marked && this->world_map[current_move_direction].noise_value > this->world_map[values[i]].noise_value) current_move_direction = values[i];
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

            if(text_direction == "START" && river_index_start == river_index_current) this->world_map[river_index_current].panel_texture = this->m_engine->resource.getTexture("panel_sea");
            else if(text_direction == "DOWN")           this->world_map[river_index_current].panel_feature_texture = &this->m_engine->resource.getTexture("panel_river_vertical");
            else if(text_direction == "DOWN AND LEFT")  this->world_map[river_index_current].panel_feature_texture = &this->m_engine->resource.getTexture("panel_river_corner_tl");
            else if(text_direction == "DOWN AND RIGHT") this->world_map[river_index_current].panel_feature_texture = &this->m_engine->resource.getTexture("panel_river_corner_tr");
            else if(text_direction == "UP")             this->world_map[river_index_current].panel_feature_texture = &this->m_engine->resource.getTexture("panel_river_vertical");
            else if(text_direction == "RIGHT AND UP")   this->world_map[river_index_current].panel_feature_texture = &this->m_engine->resource.getTexture("panel_river_corner_tl");
            else if(text_direction == "RIGHT AND DOWN") this->world_map[river_index_current].panel_feature_texture = &this->m_engine->resource.getTexture("panel_river_corner_bl");
            else if(text_direction == "LEFT")           this->world_map[river_index_current].panel_feature_texture = &this->m_engine->resource.getTexture("panel_river_horizontal");
            else if(text_direction == "LEFT AND UP")    this->world_map[river_index_current].panel_feature_texture = &this->m_engine->resource.getTexture("panel_river_corner_tr");
            else if(text_direction == "LEFT AND DOWN")  this->world_map[river_index_current].panel_feature_texture = &this->m_engine->resource.getTexture("panel_river_corner_br");
            else if(text_direction == "RIGHT")          this->world_map[river_index_current].panel_feature_texture = &this->m_engine->resource.getTexture("panel_river_horizontal");
            else if(text_direction == "UP AND LEFT")    this->world_map[river_index_current].panel_feature_texture = &this->m_engine->resource.getTexture("panel_river_corner_bl");
            else if(text_direction == "UP AND RIGHT")   this->world_map[river_index_current].panel_feature_texture = &this->m_engine->resource.getTexture("panel_river_corner_br");

            last_move_value     = current_move_value;
            river_index_current = current_move_direction;

            if(coast_found) break;

            if(this->world_map[river_index_current].is_coast) {
                coast_found = true;
            }
        }
    }
}