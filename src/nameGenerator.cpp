#include "nameGenerator.hpp"

std::string iso::generate(iso::GenerationType type, int iterations) {
    std::string name;
    for(int iteration = 0; iteration < iterations; iteration++) {
        if(iteration == 0)
            name += prefix[std::rand() % prefix.size()];

        else if(iteration == iterations - 1)
            name += suffix[std::rand() % suffix.size()];

        else if(iteration > 0 && iteration < iterations - 1)
            name += middle[std::rand() % middle.size()];
    }

    return name;
}