#include "worldGenerator.hpp"
#include "globalutilities.hpp"
#include "generationSettings.hpp"
#include "./building/building_definitions.hpp"

using namespace iso;

WorldGenerator::WorldGenerator() {

}

WorldGenerator::WorldGenerator(resourceManager* resource, Texturizer* texturizer) {
    this->resource   = resource;
    this->texturizer = texturizer;

    auto world_size = world_settings.getWorldSize();

    this->m_gradient.resize(world_size);
    this->world_map.resize(world_size);

    std::srand(std::time(0));
}

WorldGenerator::~WorldGenerator() {

}

void WorldGenerator::generateWorld() {
    std::cout << "====================\n";
    std::cout << "[World Generation]: Generating world.\n";

    this->m_region = Region();
    sf::Clock clock;
    std::vector <float> worldmap_noise(world_settings.getWorldSize());

    // Generate initial noise map.
    {
        std::vector <float> input(world_settings.getWorldSize());
        for(int i = 0; i < world_settings.getWorldSize(); i++) {
            input[i] = (float)rand() / (float)RAND_MAX;    
        }

        float biggest_noise_recorded = 1.0f;

        NoiseSettings settings = world_settings.getWorldNoiseSettings();

        for(int y = 0; y < world_settings.getWorldWidth(); y++) {
            for(int x = 0; x < world_settings.getWorldWidth(); x++) {
                const int index = world_settings.calculateWorldIndex(x, y);

                if(x < world_settings.getWorldMargin() || x > (world_settings.getWorldWidth() - world_settings.getWorldMargin()) || y < world_settings.getWorldMargin() || y > (world_settings.getWorldWidth() - world_settings.getWorldMargin())) {
                    worldmap_noise[index] = 0.0f;
                    continue;
                }

                float noise = 0.0f;
                float scale = 1.0f;
                float scale_acc = 0.0f;

                for(unsigned int o = 0; o < settings.octaves; o++) {
                    const int sampleX1 = (x / settings.persistence) * settings.persistence;
                    const int sampleY1 = (y / settings.persistence) * settings.persistence;

                    const int sampleX2 = (sampleX1 + settings.persistence) % world_settings.getWorldWidth();					
                    const int sampleY2 = (sampleY1 + settings.persistence) % world_settings.getWorldWidth();

                    const float blendX = (float)(x - sampleX1) / (float)settings.persistence;
                    const float blendY = (float)(y - sampleY1) / (float)settings.persistence;

                    const float sampleT = (1.0f - blendX) * input[sampleY1 * world_settings.getWorldWidth() + sampleX1] + blendX * input[sampleY1 * world_settings.getWorldWidth() + sampleX2];
                    const float sampleB = (1.0f - blendX) * input[sampleY2 * world_settings.getWorldWidth() + sampleX1] + blendX * input[sampleY2 * world_settings.getWorldWidth() + sampleX2];

                    scale_acc += scale;
                    noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
                    scale = float(scale / settings.bias);
                }
                
                float noise_value = (noise / scale_acc) * settings.multiplier;
                
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
        NoiseSettings settings_gradient = world_settings.getWorldGradientSettings();

        const sf::Vector2i centre = sf::Vector2i(
            world_settings.getWorldWidth() / 2,
            world_settings.getWorldWidth() / 2
        );

        for(int y = 0; y < world_settings.getWorldWidth(); y++) {
            for(int x = 0; x < world_settings.getWorldWidth(); x++) {
                const int index = world_settings.calculateWorldIndex(x, y);

                if(x < world_settings.getWorldMargin() || x > (world_settings.getWorldWidth() - world_settings.getWorldMargin()) || y < world_settings.getWorldMargin() || y > (world_settings.getWorldWidth() - world_settings.getWorldMargin())) {
                    this->m_gradient[index] = 0.0f;
                    continue;
                }

                const float distance_x = (centre.x - x) * (centre.x - x);
                const float distance_y = (centre.y - y) * (centre.y - y);

                // Both width and height of the map have a impact on distance.
                float distance = (sqrt(distance_x + distance_y) / world_settings.getWorldWidth());

                if(distance > 1.0f) distance = 1.0f;

                // Distance is bigger when further away from the centre of the gradient.
                this->m_gradient[index] = distance * settings_gradient.persistence;
            }
        }
    }

    // Generate initial worldmap divided into water tiles and terrain tiles.
    for(int y = 0; y < world_settings.getWorldWidth(); y++) {
        for(int x = 0; x < world_settings.getWorldWidth(); x++) {                
            const int index      = world_settings.calculateWorldIndex(x, y);
            const float noise    = worldmap_noise[index]; 
            const float gradient = this->m_gradient[index];
            float value          = noise - gradient;

            if(value < 0.0f) value = 0.0f;
            if(value > 1.0f) value = 1.0f;

            if(value > world_settings.getWorldTerrainMin())
                this->m_region.regiontype.set_terrain();
        
            else
                this->m_region.regiontype.set_ocean();

            this->m_region.height            = std::ceil(value * 100) / 100;;
            this->m_region.object_position.x = world_settings.panelSize() * x; 
            this->m_region.object_position.y = world_settings.panelSize() * y;
            this->m_region.object_size       = sf::Vector2f(world_settings.panelSize(), world_settings.panelSize());

            this->world_map[index] = this->m_region;
        }
    }

    // Find and mark coast tiles.
    // This loop marks tiles from 8 cardinal directions.
    for(int y = 0; y < world_settings.getWorldWidth(); y++) {
        for(int x = 0; x < world_settings.getWorldWidth(); x++) {
            const int index = world_settings.calculateWorldIndex(x, y);
            
            if(this->world_map[index].regiontype.is_terrain()) {
                const int index_up    = index - world_settings.getWorldWidth();             
                const int index_down  = index + world_settings.getWorldWidth();
                const int index_left  = index - 1;
                const int index_right = index + 1;
                const int index_tl    = index - 1 - world_settings.getWorldWidth();
                const int index_tr    = index + 1 - world_settings.getWorldWidth();
                const int index_bl    = index - 1 + world_settings.getWorldWidth();
                const int index_br    = index + 1 + world_settings.getWorldWidth();

                if(index_up > 0)
                    if(!this->world_map[index_up].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();

                if(index_left > 0)
                    if(!this->world_map[index_left].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();

                if(index_down < world_settings.getWorldSize())
                    if(!this->world_map[index_down].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();

                if(index_right < world_settings.getWorldSize())
                    if(!this->world_map[index_right].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();

                if(index_tl > 0)
                    if(!this->world_map[index_tl].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();

                if(index_tr > 0)
                    if(!this->world_map[index_tr].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();

                if(index_bl < world_settings.getWorldSize())
                    if(!this->world_map[index_bl].regiontype.is_terrain()) 
                        this->world_map[index].regiontype.set_coast();

                if(index_br < world_settings.getWorldSize())
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
        for(int y = 0; y < world_settings.getWorldWidth(); y++) {
            for(int x = 0; x < world_settings.getWorldWidth(); x++) {
                const int index = world_settings.calculateWorldIndex(x, y);
                auto& region = this->world_map[index];

                if(region.regiontype.is_terrain()) {
                    int terrain_tiles = 0;
    
                    if(index - 1 > 0)
                        if(this->world_map[index - 1].regiontype.is_terrain())
                            terrain_tiles++;
    
                    if(index + 1 < world_settings.getWorldSize())
                        if(this->world_map[index + 1].regiontype.is_terrain())
                            terrain_tiles++; 
    
                    if(index - world_settings.getWorldWidth() > 0)
                        if(this->world_map[index - world_settings.getWorldWidth()].regiontype.is_terrain())
                            terrain_tiles++;
    
                    if(index + world_settings.getWorldWidth() < world_settings.getWorldSize())
                        if(this->world_map[index + world_settings.getWorldWidth()].regiontype.is_terrain())
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
    for(int y = 0; y < world_settings.getWorldWidth(); y++) {
        for(int x = 0; x < world_settings.getWorldWidth(); x++) {
            const int index = world_settings.calculateWorldIndex(x, y);
            auto& region    = this->world_map[index];

            region.object_texture_name = this->getWorldmapTile(index);
        }
    }

    std::cout << "[World Generation]: Generating world features.\n";

    this->worldmapGenerateRivers();    
    this->worldmapGenerateForests();

    // Generate lakes.
    // Generation is random.
    for(int t = 0; t < world_settings.getWorldWidth() / 10; t++) {
        int index           = 0;
        auto& region        = this->world_map[index];
        bool conditions_met = false;

        while(!conditions_met) {
            index  = rand() % world_settings.getWorldSize();
            region = this->world_map[index];

            if(!region.regiontype.is_ocean() && !region.regiontype.is_forest() && !region.regiontype.is_river() && !this->is_lake(index) && !region.regiontype.is_coast()) {
                this->lakes[index] = GameObject(region.getPosition(), sf::Vector3f(), region.getSize(), this->getTileVariation("panel_lake"));
                conditions_met = true;
            }
        }
    }

    const float time_rounded = std::ceil(clock.getElapsedTime().asSeconds() * 100) / 100;
    std::cout << "[World Generation]: World generated in " << time_rounded << "s.\n";
    std::cout << "====================\n";
}

void WorldGenerator::worldmapGenerateClimate() {
    std::cout << "[World Generation]: Generating climate.\n";

    // Calculate LATITUDE.
    for(int y = 0; y < world_settings.getWorldWidth(); y++) {
        for(int x = 0; x < world_settings.getWorldWidth(); x++) {       
            const int index = world_settings.calculateWorldIndex(x, y);

            // Latitude for the upper half.
            if(y < world_settings.getWorldWidth() / 2)
                this->world_map[index].latitude = float(y + 1) / float(world_settings.getWorldWidth() / 2);

            // Latitude for the bottom half.
            else {
                const int i = y - world_settings.getWorldWidth() / 2;
                const int reversed_height = y - 2 * i;
                this->world_map[index].latitude = float(reversed_height) / float(world_settings.getWorldWidth() / 2);
            }
        }
    }
    
    std::cout << "  [] Calculated latitude.\n";
    
    // Calculate TEMPERATURE.
    for(int y = 0; y < world_settings.getWorldWidth(); y++) {
        for(int x = 0; x < world_settings.getWorldWidth(); x++) {            
            const int panel_index = world_settings.calculateWorldIndex(x, y);
            Region& panel = this->world_map[panel_index];

            // Panel temperature zone classification.
            const bool PANEL_ARCTIC       = (y < world_settings.getWorldMarginPolar() || y > world_settings.getWorldWidth() - world_settings.getWorldMarginPolar() - 1) ? true : false;
            const bool PANEL_NEAR_POLE    = ((y < world_settings.getWorldMarginPolar() + world_settings.getWorldMargin() + 0.1f * (float)world_settings.getWorldWidth() && y >= world_settings.getWorldMarginPolar()) || (y >= world_settings.getWorldWidth() - (world_settings.getWorldMarginPolar() + world_settings.getWorldMargin() + 0.1f * (float)world_settings.getWorldWidth()) && y < world_settings.getWorldWidth() - world_settings.getWorldMarginPolar())) ? true : false;
            const bool PANEL_NEAR_EQUATOR = (y > world_settings.getWorldWidth() / 2 - 1 - world_settings.getWorldMargin() && y <= world_settings.getWorldWidth() / 2 + 1 + world_settings.getWorldMargin()) ? true : false;

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
    std::vector <float> input(world_settings.getWorldSize());
    for(int i = 0; i < world_settings.getWorldSize(); i++) {
        input[i] = (float)rand() / (float)RAND_MAX;
    }
    
    float biggest_noise_recorded = 1.0f;
    NoiseSettings settings_moisture = world_settings.getWorldMoistureSettings();

    for(int y = 0; y < world_settings.getWorldWidth(); y++) {
        for(int x = 0; x < world_settings.getWorldWidth(); x++) {
            const int index = world_settings.calculateWorldIndex(x, y);

            if(!this->world_map[index].regiontype.is_terrain())
                this->world_map[index].moisture = 1.0f;

            else if(this->world_map[index].regiontype.is_terrain()) {
                float noise = 0.0f;
                float scale = 1.0f;
                float scale_acc = 0.0f;

                for(unsigned int o = 0; o < settings_moisture.octaves; o++) {
                    const int sampleX1 = (x / settings_moisture.persistence) * settings_moisture.persistence;
                    const int sampleY1 = (y / settings_moisture.persistence) * settings_moisture.persistence;

                    const int sampleX2 = (sampleX1 + settings_moisture.persistence) % (int)world_settings.getWorldWidth();					
                    const int sampleY2 = (sampleY1 + settings_moisture.persistence) % (int)world_settings.getWorldWidth();

                    const float blendX = (float)(x - sampleX1) / (float)settings_moisture.persistence;
                    const float blendY = (float)(y - sampleY1) / (float)settings_moisture.persistence;

                    const float sampleT = (1.0f - blendX) * input[sampleY1 * world_settings.getWorldWidth() + sampleX1] + blendX * input[sampleY1 * world_settings.getWorldWidth() + sampleX2];
                    const float sampleB = (1.0f - blendX) * input[sampleY2 * world_settings.getWorldWidth() + sampleX1] + blendX * input[sampleY2 * world_settings.getWorldWidth() + sampleX2];

                    scale_acc += scale;
                    noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
                    scale = float(scale / settings_moisture.bias);
                }
                
                float noise_value = (noise / scale_acc) * settings_moisture.multiplier;
                
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

                this->world_map[y * world_settings.getWorldWidth() + x].moisture = noise_value;
            }
        }
    }

    std::cout << "  [] Calculated moisture.\n";
}

void WorldGenerator::worldmapAssignBiome() {
    std::cout << "[World Generation]: Generating biomes.\n";
    
    for(int index = 0; index < world_settings.getWorldSize(); index++) {
        Region& panel = this->world_map[index];

        if(!panel.regiontype.is_terrain())
            panel.biome = BIOME_OCEAN;
        
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
                panel.biome = BIOME_MEDITERRANEAN;

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
    std::vector <int> river_origin_index(world_settings.getWorldRiverQuantity());
    const int area_around_origin = world_settings.getWorldRiverScan();

    for(unsigned int river_number = 0; river_number < world_settings.getWorldRiverQuantity(); river_number++) {
        int possible_river_origin_index = rand() % world_settings.getWorldSize() - 1;
        bool place_found = false;

        while(!place_found) {
            int index = rand() % world_settings.getWorldSize() - 1;
            
            if(this->world_map[index].regiontype.is_terrain() && !this->world_map[index].regiontype.is_coast() && !this->world_map[index].regiontype.is_forest()) {
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
                    const int index = possible_river_origin_index + world_settings.calculateWorldIndex(x, y);

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
        if(rivers_verified == world_settings.getWorldRiverQuantity()) {
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

            const int index_up    = river_index_current - world_settings.getWorldWidth();
            const int index_down  = river_index_current + world_settings.getWorldWidth();
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
                    if(current_move_value + last_move_value == -world_settings.getWorldWidth() + 1) {
                        direction = RiverDirection::RIVER_NORTH_TO_WEST;
                    }

                    // UP AND LEFT
                    if(current_move_value + last_move_value == -world_settings.getWorldWidth() - 1) {
                        direction = RiverDirection::RIVER_NORTH_TO_EAST;
                    }

                    // UP
                    if(current_move_value == -world_settings.getWorldWidth() && last_move_value == -world_settings.getWorldWidth()) {
                        direction = RiverDirection::RIVER_VERTICAL;
                    }

                }

                else if(current_move_direction == index_down) {
                    // DOWN AND RIGHT
                    if(current_move_value + last_move_value == world_settings.getWorldWidth() + 1) {
                        direction = RiverDirection::RIVER_SOUTH_TO_WEST;
                    }

                    // DOWN AND LEFT
                    if(current_move_value + last_move_value == world_settings.getWorldWidth() - 1) {
                        direction = RiverDirection::RIVER_SOUTH_TO_EAST;
                    }

                    // DOWN
                    if(current_move_value == world_settings.getWorldWidth() && last_move_value == world_settings.getWorldWidth()) {
                        direction = RiverDirection::RIVER_VERTICAL;
                    }
                }

                else if(current_move_direction == index_left) {
                    // LEFT
                    if(current_move_value + last_move_value == -1 || -2) {
                        direction = RiverDirection::RIVER_HORIZONTAL;
                    }

                    // LEFT AND UP
                    if(current_move_value + last_move_value == -1 - world_settings.getWorldWidth()) {
                        direction = RiverDirection::RIVER_SOUTH_TO_WEST;
                    }

                    // LEFT AND DOWN
                    if(current_move_value + last_move_value == -1 + world_settings.getWorldWidth()) {
                        direction = RiverDirection::RIVER_NORTH_TO_WEST;
                    }

                }

                else if(current_move_direction == index_right) {
                    // RIGHT
                    if(current_move_value + last_move_value == 1 || 2) {
                        direction = RiverDirection::RIVER_HORIZONTAL;
                    }

                    // RIGHT AND UP
                    if(current_move_value + last_move_value == 1 - world_settings.getWorldWidth()) {
                        direction = RiverDirection::RIVER_SOUTH_TO_EAST;
                    }

                    // RIGHT AND DOWN
                    if(current_move_value + last_move_value == 1 + world_settings.getWorldWidth()) {
                        direction = RiverDirection::RIVER_NORTH_TO_EAST;
                    }
                }

                else 
                    direction = RiverDirection::RIVER_NONE; 
            }

            Region& panel     = this->world_map[river_index_current];
            GameObject& river = this->rivers[river_index_current];
            const auto panel_position     = panel.getPosition();
            const auto panel_offset       = sf::Vector3f(); 
            const sf::Vector2f panel_size = sf::Vector2f(world_settings.panelSize(), world_settings.panelSize()); 

            this->texturizer->createColouredWorldmapTexture("panel_full", "panel_full_lightblue", COLOUR_CYAN, COLOUR_TRANSPARENT);
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
                        this->rivers[river_index_start] = GameObject(start_panel.getPosition(), panel_offset, start_panel.getSize(), this->getRiverTileVariation("panel_river_origin_right"));

                else if(river_index_current - world_settings.getWorldWidth() > 0)
                    if(this->world_map[river_index_current - world_settings.getWorldWidth()].regiontype.is_river())
                        this->rivers[river_index_start] = GameObject(start_panel.getPosition(), panel_offset, start_panel.getSize(), this->getRiverTileVariation("panel_river_origin_down"));
                
                else if(river_index_current + 1 < world_settings.getWorldSize())
                    if(this->world_map[river_index_current + 1].regiontype.is_river())
                        this->rivers[river_index_start] = GameObject(start_panel.getPosition(), panel_offset, start_panel.getSize(), this->getRiverTileVariation("panel_river_origin_left"));
                
                else if(river_index_current + world_settings.getWorldWidth() < world_settings.getWorldSize())
                    if(this->world_map[river_index_current + world_settings.getWorldWidth()].regiontype.is_river())
                        this->rivers[river_index_start] = GameObject(start_panel.getPosition(), panel_offset, start_panel.getSize(), this->getRiverTileVariation("panel_river_origin_up"));
            }

            if(panel.regiontype.is_coast()) {
                if(river_index_current - 1 > 0)
                    if(this->world_map[river_index_current - 1].regiontype.is_river())
                        this->rivers[river_index_current] = GameObject(panel.getPosition(), panel_offset, panel.getSize(), this->getRiverTileVariation("panel_river_estuary_right"));

                if(river_index_current - world_settings.getWorldWidth() > 0)
                    if(this->world_map[river_index_current - world_settings.getWorldWidth()].regiontype.is_river())
                        this->rivers[river_index_current] = GameObject(panel.getPosition(), panel_offset, panel.getSize(), this->getRiverTileVariation("panel_river_estuary_down"));

                if(river_index_current + 1 < world_settings.getWorldSize())
                    if(this->world_map[river_index_current + 1].regiontype.is_river())
                        this->rivers[river_index_current] = GameObject(panel.getPosition(), panel_offset, panel.getSize(), this->getRiverTileVariation("panel_river_estuary_left"));

                if(river_index_current + world_settings.getWorldWidth() < world_settings.getWorldSize())
                    if(this->world_map[river_index_current + world_settings.getWorldWidth()].regiontype.is_river())
                        this->rivers[river_index_current] = GameObject(panel.getPosition(), panel_offset, panel.getSize(), this->getRiverTileVariation("panel_river_estuary_up"));
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

    NoiseContainer noise_forest;
    NoiseSettings settings = world_settings.getWorldForestSettings(); 

    this->generateNoise(settings, noise_forest);

    for(int index = 0; index < noise_forest.size(); index++) {
        const float noise  = noise_forest[index];

        Region& region = this->world_map[index];

        const bool is_ocean   = this->is_ocean(index);
        const bool is_river   = region.regiontype.is_river();
        const bool is_coast   = region.regiontype.is_coast();

        // If noise is high enough and tile is valid, place a forest. 
        if(noise > world_settings.getWorldForestMin() && !is_ocean && !is_coast && !is_river) {
            region.regiontype.set_forest();            
            const std::string texture_name = this->getWorldmapTreeTextureName(region.biome);
            this->forests[index] = GameObject(region.getPosition(), sf::Vector3f(), region.getSize(), texture_name);
        }
    }
}

void WorldGenerator::generateNoise(NoiseSettings& settings, NoiseContainer& storage) {
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
            
            if(buffer_noise > amplifier) 
                amplifier = buffer_noise;
            buffer_noise *= amplifier;

            storage[index] = (buffer_noise > 1.0f) ? 1.0f : buffer_noise; 
        }
    }
}

// This function returns grid coordinate of a tile.
// The range on these is from (for both x and y) (0, region_size).
// Return value from this function is accepted by tilePositionScreen() to convert back to tile's original position.
sf::Vector2i WorldGenerator::tileGridPosition(sf::Vector2f tile_position) const {
    sf::Vector2i cell(
        tile_position.x / world_settings.tileSize().x,
        tile_position.y / world_settings.tileSize().y
    );

    sf::Vector2i selected(
        (cell.y - world_settings.tileOffset().y) + (cell.x - world_settings.tileOffset().x),
        (cell.y - world_settings.tileOffset().y) - (cell.x - world_settings.tileOffset().x)
    );

    sf::Vector2i position_within_tile(
        (int)tile_position.x % (int)world_settings.tileSize().x,
        (int)tile_position.y % (int)world_settings.tileSize().y
    );

    auto colour_name = this->getTilePixelColour(position_within_tile);
    if(colour_name == "Red")
        selected += sf::Vector2i(-1, 0);

    if(colour_name == "Green")
        selected += sf::Vector2i(1, 0);

    if(colour_name == "Blue")
        selected += sf::Vector2i(0, -1);
    
    if(colour_name == "Yellow")
        selected += sf::Vector2i(1, 0);

    return selected;
}

sf::Vector2i WorldGenerator::tileGridPosition(int index) const {
    return sf::Vector2i(
        index % world_settings.getRegionWidth(),
        index / world_settings.getRegionWidth()
    );
}

// This function accepts coordinates of the tile in a grid - not the tile position.
// The first tile's coordinates in a grid are (0,0), but the position might be (128, 128) or some other point based on the offset. 
sf::Vector3f WorldGenerator::tilePositionScreen(int x, int y) {
    return sf::Vector3f(
        (world_settings.tileOffset().x * world_settings.tileSize().x) + (x - y) * (world_settings.tileSize().x / 2),
        (world_settings.tileOffset().y * world_settings.tileSize().y) + (x + y) * (world_settings.tileSize().y / 2),
        0
    );
}

sf::Vector3f WorldGenerator::tilePositionScreen(sf::Vector2i grid_position) {
    return this->tilePositionScreen(grid_position.x, grid_position.y);
}

sf::Vector3f WorldGenerator::tilePositionScreen(sf::Vector2f grid_position) {
    return this->tilePositionScreen(sf::Vector2i(grid_position.x, grid_position.y));
}

bool WorldGenerator::is_biome(int region_index, Biome biome) const {
    return this->world_map[region_index].biome.biome_name == biome.biome_name;
}

bool WorldGenerator::is_ocean(int region_index) const {
    return this->is_biome(region_index, BIOME_OCEAN);
}

bool WorldGenerator::is_tropical(int region_index) const {
    return this->is_biome(region_index, BIOME_TROPICAL);
}

bool WorldGenerator::is_mediterranean(int region_index) const {
    return this->is_biome(region_index, BIOME_MEDITERRANEAN);
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

bool WorldGenerator::is_terrain(int region_index) const {
    return region_index > world_settings.getWorldTerrainMin();
}

bool WorldGenerator::is_coast(int region_index) const {
    return this->world_map.at(region_index).regiontype.is_coast();
}

bool WorldGenerator::is_lake(int region_index) const {
    return this->lakes.count(region_index);
}

bool WorldGenerator::is_river(int region_index) const {
    return this->rivers.count(region_index);
}

std::string WorldGenerator::getTilePixelColour(sf::Vector2i pixel) {
    if(pixel.x < 0 || pixel.y < 0) 
        return "Other";

    if(pixel.x > world_settings.tileSize().x - 1 || pixel.y > world_settings.tileSize().y - 1) 
        return "Other";

    auto pixel_colour = this->resource->getTexture("tile_template_direction").copyToImage().getPixel(pixel.x, pixel.y);

    if(pixel_colour == sf::Color::Red)         return "Red";
    else if(pixel_colour == sf::Color::Green)  return "Green";
    else if(pixel_colour == sf::Color::Blue)   return "Blue";
    else if(pixel_colour == sf::Color::Yellow) return "Yellow";
    else return "Other";
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

    auto world_size = world_settings.getWorldSize();

    // LEFT
    if(world_settings.inWorldBounds(index - 1)) {
        if(this->world_map[index - 1].regiontype.is_terrain()) {
            LEFT = true;
        }
    }

    // RIGHT
    if(world_settings.inWorldBounds(index + 1)) {
        if(this->world_map[index + 1].regiontype.is_terrain()) {
            RIGHT = true;
        }
    }

    // TOP
    if(world_settings.inWorldBounds(index - world_settings.getWorldWidth())) {
        if(this->world_map[index - world_settings.getWorldWidth()].regiontype.is_terrain()) {
            TOP = true;
        }
    }

    // TOP LEFT
    if(world_settings.inWorldBounds(index - 1 - world_settings.getWorldWidth())) {
        if(this->world_map[index - 1 - world_settings.getWorldWidth()].regiontype.is_terrain()) {
            TOPLEFT = true;
        }
    }
    
    // TOP RIGHT
    if(world_settings.inWorldBounds(index + 1 - world_settings.getWorldWidth())) {
        if(this->world_map[index + 1 - world_settings.getWorldWidth()].regiontype.is_terrain()) {
            TOPRIGHT = true;
        }
    }

    // BOTTOM
    if(world_settings.inWorldBounds(index + world_settings.getWorldWidth())) {
        if(this->world_map[index + world_settings.getWorldWidth()].regiontype.is_terrain()) {
            BOTTOM = true;
        }
    }
    
    // BOTTOM LEFT
    if(world_settings.inWorldBounds(index - 1 + world_settings.getWorldWidth())) {
        if(this->world_map[index - 1 + world_settings.getWorldWidth()].regiontype.is_terrain()) {
            BOTTOMLEFT = true;
        }
    }
    
    // BOTTOM RIGHT
    if(world_settings.inWorldBounds(index + 1 + world_settings.getWorldWidth())) {
        if(this->world_map[index + 1 + world_settings.getWorldWidth()].regiontype.is_terrain()) {
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

    if(biome == BIOME_MEDITERRANEAN)
        return "panel_tree_mediterranean_1";

    if(biome == BIOME_TROPICAL)
        return "panel_tree_tropical_1";

    return "default";
}

std::string WorldGenerator::getRiverTileVariation(const std::string& id) const {
    // Rest of the tiles have 3 variations each.
    const int panel_type = rand() % 3;
    
    // Worldmap texture naming convention is: <panel_type><id>.
    // So the function returns for example: "panel_river_horizontal_0".
    return id + "_" + std::to_string(panel_type);
}

void WorldGenerator::generateRegion(int region_index) {
    sf::Clock clock;

    auto& region = this->world_map[region_index];
    
    // Do not generate the region that was generated already.
    if(region.visited)
        return;

    region.map.resize(world_settings.getRegionSize());

    this->m_tile.object_size = world_settings.tileSize();

    /* Region generation steps:
     * 1. Read every pixel from the region texture. If the pixel is BLACK, place terrain. If the pixel is WHITE, place water.
     * 2. If a river or lake exists in the region, read every pixel from the river texture and place them in the appropriate position on the region map.
     * 3. If the region is forest, generate a noise map for the forest. If a tile is above a certain threshold, place a tree. 
     * 4. Place other region resources. */

    const std::string is_coast  = region.regiontype.is_coast()    ? "True" : "False";
    const std::string is_river  = region.regiontype.is_river()    ? "True" : "False";
    const std::string is_lake   = this->lakes.count(region_index) ? "True" : "False";
    const std::string is_forest = region.regiontype.is_forest()   ? "True" : "False";

    std::cout << "====================\n";
    std::cout << "[World Generation]: Generating region " << region_index << ".\n";
    std::cout << "  [] Biome: \t" << region.biome.biome_name << "\n";
    std::cout << "  [] Coast: \t" << is_coast  << "\n";
    std::cout << "  [] Lake:  \t" << is_lake   << "\n";
    std::cout << "  [] River: \t" << is_river  << "\n";
    std::cout << "  [] Forest:\t" << is_forest << "\n";

    const auto& region_texture_name = region.getTextureName();
    const auto& base_texture        = this->extractBaseTexture(region_texture_name, region.biome);     
    const auto region_image         = this->resource->getTexture(base_texture).copyToImage();    
    const std::string biome_tile    = region.biome.getTile();
    for(int y = 0; y < world_settings.getRegionWidth(); y++) {
        for(int x = 0; x < world_settings.getRegionWidth(); x++) {
            const int index   = world_settings.calculateRegionIndex(x, y);
            const auto colour = region_image.getPixel(x, y);
            
            this->m_tile.object_position = this->tilePositionScreen(x, y);

            // Read terrain data.
            if(colour == COLOUR_WHITE) {
                this->m_tile.tiletype.set_ocean();
                this->m_tile.setElevation(0);
                this->m_tile.object_texture_name = "tile_ocean";
            }

            else if(colour == COLOUR_BLACK) {
                this->m_tile.tiletype.set_terrain();
                this->m_tile.setElevation(1);
                this->m_tile.object_texture_name = biome_tile;
            }

            region.map[index] = this->m_tile;
        }
    }

    const bool river_exists = this->is_river(region_index);
    if(river_exists) {
        const auto& river = this->rivers[region_index];
        const auto river_image = this->resource->getTexture(river.getTextureName()).copyToImage();

        // Because rivers and oceans may be in the same region there will not be a seemless transition, because both tile types have different textures.
        const std::string water_tile = this->extractBaseTexture(region.getTextureName(), region.biome) == "panel_full" ? "tile_river" : "tile_ocean";
    
        for(int y = 0; y < world_settings.getRegionWidth(); y++) {
            for(int x = 0; x < world_settings.getRegionWidth(); x++) {
                const int index   = world_settings.calculateRegionIndex(x, y);
                const auto colour = river_image.getPixel(x, y);
                auto& tile        = region.map[index];

                if(colour == COLOUR_BLUE_RIVER) {
                    if(!tile.tiletype.is_ocean())
                        tile.tiletype.set_river();                    

                    else
                        tile.tiletype.set_ocean();

                    tile.object_texture_name = "tile_ocean";
                    tile.setElevation(0);
                }
            }
        }   
    }

    const bool lake_exists = this->is_lake(region_index);
    if(lake_exists) {
        const auto& lake = this->lakes[region_index];
        const auto lake_image = this->resource->getTexture(lake.getTextureName()).copyToImage();

        for(int y = 0; y < world_settings.getRegionWidth(); y++) {
            for(int x = 0; x < world_settings.getRegionWidth(); x++) {
                const int index   = world_settings.calculateRegionIndex(x, y);
                const auto colour = lake_image.getPixel(x, y);
                auto& tile        = region.map[index];

                if(colour == COLOUR_BLUE_RIVER) {
                    if(!tile.tiletype.is_ocean())
                        tile.tiletype.set_river();
                    tile.setElevation(0);
                    tile.object_texture_name = "tile_river";
                }
            }
        }  
    }
    
    // Fill in blank spots created by tile elevation.
    {
        for(int y = 0; y < world_settings.getRegionWidth(); y++) {
            for(int x = 0; x < world_settings.getRegionWidth(); x++) {
                const int index = world_settings.calculateRegionIndex(x, y);
                const auto& current_tile = region.map[index]; 
                
                // Elevation can not be lower than 0.
                if(current_tile.getElevation() == 0)
                    continue;
                
                const int index_left   = index - 1;
                const int index_right  = index + 1;
                const int index_top    = index - world_settings.getRegionWidth();  
                const int index_bottom = index + world_settings.getRegionWidth();  

                // Check only for the tile to the right tile and the bottom tile, since the left and top elevation would not be visible.

                int biggest_difference = 0;
                if(world_settings.inRegionBounds(index_right)) {
                    const int height_difference = std::abs(current_tile.getElevation() - region.map[index_right].getElevation());
                    if(biggest_difference < height_difference)
                        biggest_difference = height_difference;
                }

                if(world_settings.inRegionBounds(index_bottom)) {
                    const int height_difference = std::abs(current_tile.getElevation() - region.map[index_bottom].getElevation());
                    if(biggest_difference < height_difference)
                        biggest_difference = height_difference;
                }

                region.sides[index].resize(biggest_difference);

                const auto side_offset = [](int index) {
                    auto offset = sf::Vector3f();
                    offset.z = (index + 1) * world_settings.tileSize().y / 2;
                    return offset;
                };

                auto tile_position = current_tile.getPosition();

                // Make sure that tiles with varying elevations have a side texture filled in.
                for(int i = 0; i < biggest_difference; i++) {
                    std::string side_texture = i > 2 ? "tile_height_stone" : "tile_height_dirt";
                    region.sides[index][i] = GameObject(tile_position, side_offset(i), current_tile.getSize(), side_texture);  
                }

                // Make sure that tiles on the sides of the region have filled in elevation.
                // Tiles along Y axis.
                if(y == world_settings.getRegionWidth() - 1) {
                    const int height_difference = region.map[index].getElevation();
                    region.sides[index].resize(height_difference);

                    for(int i = 0; i < height_difference; i++) {
                        std::string side_texture = i > 2 ? "tile_height_stone" : "tile_height_dirt";
                        region.sides[index][i] = GameObject(tile_position, side_offset(i), current_tile.getSize(), side_texture);
                    }
                }

                // Tiles along X axis.
                if(x == world_settings.getRegionWidth() - 1) {
                    const int height_difference = region.map[index].getElevation();
                    region.sides[index].resize(height_difference);

                    for(int i = 0; i < height_difference; i++) {
                        std::string side_texture = i > 2 ? "tile_height_stone" : "tile_height_dirt";
                        region.sides[index][i] = GameObject(tile_position, side_offset(i), current_tile.getSize(), side_texture);
                    }
                }
            }
        }
    }

    // Generate region resources.
    // Generate trees.
    {
        NoiseContainer container;
        NoiseSettings settings;

        region.regiontype.is_forest()
            ? settings = world_settings.getRegionForestSettingsDense()
            : settings = world_settings.getRegionForestSettingsSparse();

        this->generateNoise(settings, container);
        for(int i = 0; i < container.size(); i++) {
            auto& tile = region.map[i];
            float noise = container[i];

            if(noise > world_settings.getRegionTreeMin() && tile.tiletype.is_terrain()) {
                const auto& tree_texture = region.biome.getTree();
                const auto texture_size  = this->resource->getTextureSize(tree_texture);
                
                // Offset is for trees that are bigger than tile size.
                auto tree_offset = sf::Vector3f(0, 0, 0);
                if(texture_size.y > world_settings.tileSize().y)
                    tree_offset.y = -texture_size.y + world_settings.tileSize().y;

                if(texture_size.x > world_settings.tileSize().x)
                    tree_offset.x = -world_settings.tileSize().x / 2;
                
                const auto tree_size = texture_size;
                region.trees[i] = GameObject(tile.getPosition(), tree_offset, tree_size, tree_texture);
            }
        }
    }

    // Generate flint.
    std::string has_flint = "False";
    bool did_flint_generate = this->regionGenerateResource(region, "tile_resource_flint", world_settings.getRegionFlintChance(), world_settings.getRegionFlintRadius());
    if(did_flint_generate)
        has_flint = "True";

    std::cout << "  [] Flint:\t" << has_flint << "\n";

    // Generate stone.
    std::string has_stone = "False";
    bool did_stone_generate = this->regionGenerateResource(region, "tile_resource_stone", world_settings.getRegionStoneChance(), world_settings.getRegionStoneRadius());
    if(did_stone_generate)
        has_stone = "True";

    std::cout << "  [] Stone:\t" << has_stone << "\n";

    std::string did_animal_generate = "False";
    bool did_animal_spot_generate = (rand() % 100) > 10;
    if(did_animal_spot_generate) {
        did_animal_generate = "True";

        bool animal_spot_valid = false;
        while(!animal_spot_valid) {
            int index = rand() % world_settings.getRegionSize();
            auto grid_position = this->tileGridPosition(index);

            if(region.isPositionValid(BUILDING_ANIMAL_SPOT, grid_position)) {
                auto texture_name = BUILDING_ANIMAL_SPOT.getTextureName();
                auto texture_size = this->resource->getTextureSize(texture_name);
                region.placeBuilding(BUILDING_ANIMAL_SPOT, texture_size, grid_position);
                animal_spot_valid = true;
            }
        }
    } 

    std::cout << "  [] Animals:\t" << did_animal_generate << "\n";

    region.visited = true;

    const float time_rounded = std::ceil(clock.getElapsedTime().asSeconds() * 100) / 100;
    std::cout << "[World Generation]: Region generated in " << time_rounded << "s.\n"; 
    std::cout << "====================\n";
}

std::string WorldGenerator::extractBaseTexture(const std::string& id, const Biome& biome) const {
    const std::string biome_suffix = "_" + toLower(biome.biome_name);
    
    const int id_length           = id.length();
    const int biome_suffix_length = biome_suffix.length();
    const int end_index = id_length - biome_suffix_length;

    std::string extract_string = "";
    for(int i = 0; i < end_index; i++)
        extract_string.append(1, id[i]);    
    return extract_string;
}

// This function generates resources in patches.
// Only one patch of a resource may be generated per region.
bool WorldGenerator::regionGenerateResource(Region& region, const std::string& resource_tile_texture, float chance, int radius) {
    const int range = 100;

    // Random value should be in range (0, 1)
    // To achieve this, generate a value between (0, 100) and divide by 100.
    const float random_value = (rand() % range) / (float)range;
    
    if(random_value < chance) {
        NoiseContainer container;
        NoiseSettings  settings = world_settings.getRegionResourceSettings();
        this->generateNoise(settings, container);
        
        int random_index = -1;
        while(true) {
            random_index = rand() % world_settings.getRegionSize();
            
            if(world_settings.inRegionBounds(random_index))
                if(region.map[random_index].tiletype.is_terrain())
                    break;
        }

        const auto& tile_selected = region.map[random_index];
        auto tile_selected_grid = this->tileGridPosition(tile_selected.getPosition2D());
        
        int angle = 0;

        // Without these checks X and Y can be smaller than 0 or bigger than region size.
        // Because of that you have to check the bounds.
        // You can not do that inside the loop because it would make it infinite.

        int min_x = tile_selected_grid.x - radius * radius < 0 ? 0 : tile_selected_grid.x - radius * radius;
        int min_y = tile_selected_grid.y - radius * radius < 0 ? 0 : tile_selected_grid.y - radius * radius;
        int max_x = tile_selected_grid.x + radius * radius >= world_settings.getRegionWidth() ? world_settings.getRegionWidth() : tile_selected_grid.x + radius * radius;
        int max_y = tile_selected_grid.y + radius * radius >= world_settings.getRegionWidth() ? world_settings.getRegionWidth() : tile_selected_grid.y + radius * radius;

        for(int y = min_y; y < max_y; y++) {
            for(int x = min_x; x < max_x; x++) {        
                const int index = world_settings.calculateRegionIndex(x, y);
                auto& tile      = region.map[index];
                
                const auto tile_centre = tile_selected.getPosition2D();
                const auto tile_point  = tile.getPosition2D();
                const auto grid_centre = sf::Vector2f(
                    this->tileGridPosition(tile_centre).x,
                    this->tileGridPosition(tile_centre).y
                );

                const auto grid_point  = sf::Vector2f(
                    this->tileGridPosition(tile_point).x,
                    this->tileGridPosition(tile_point).y
                );
                
                const int radius_modified = radius * (1.00f + container[angle]);
                const bool point_inside = inCircle(grid_point, grid_centre, radius_modified);
                if(point_inside && tile.tiletype.is_terrain() && !region.isTree(index)) {
                    tile.object_texture_name = resource_tile_texture;
                    angle++;
                }
            }
        }

        return true;
    }

    return false;
}