#include "nameGenerator.hpp"

std::string iso::generate(iso::GenerationType type, int iterations) {
    std::string name;
    
    switch(type) {
        default:
            break;
        
        case iso::GenerationType::COUNTRY: {
            for(int iteration = 0; iteration < iterations; iteration++) {
                if(iteration == 0)
                    name += country_prefix[std::rand() % country_prefix.size()];
    
                else if(iteration == iterations - 1)
                    name += country_suffix[std::rand() % country_suffix.size()];
    
                else if(iteration > 0 && iteration < iterations - 1)
                    name += country_middle[std::rand() % country_middle.size()];
            }
            
            break;
        }
    
        case iso::GenerationType::CITY: {
            name = city_names[rand() % city_names.size()];
            break;
        }
    }

    return name;
}