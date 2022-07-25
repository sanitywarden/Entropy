#include "animal.hpp"
#include "region.hpp"
#include "generationSettings.hpp"
#include "globalutilities.hpp"
#include "resource_definitions.hpp"

#include <iostream>

using namespace iso;

Animal::Animal()
    : Building(BUILDINGSIZE128X128, "animalspot_deer", "Animal Spot", "icon_default", 8, VECTOR2X2, VECTOR1X1, false),
      Resource("Animal", ResourceType::TYPE_ANIMAL_FOOD, "icon_item_wood", 0, 2)
{}

Animal::~Animal() 
{}

bool Animal::isGenerationSpotValid(GameObject* object, int index) const {
    auto* region = static_cast<Region*>(object);

    // Do that because this building is bigger than 1x1.
    for(int y = 0; y < this->getBuildingArea().y; y++) {
        for(int x = 0; x < this->getBuildingArea().x; x++) {
            int index_loop = world_settings.calculateRegionIndex(x, y);

            if(region->isTree(index))
                return false;

            if(region->map[index].tiletype.is_water())
                return false;

            if(!startsWith(region->map[index].object_texture_name, "tile_grass"))    
                return false; 
        }
    }

    return true;
}

void Animal::placeResource(GameObject* object, int index) const {
    auto* region = static_cast<Region*>(object);
    region->placeBuilding(*this, this->getSize(), tileGridPosition(index));
    region->map[index].resource = getResourcePointer("Animal Spot");
}

void Animal::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::VertexArray game_object(sf::Quads, 4);

    auto position2d = this->getPosition2D();
    auto size       = this->getSize();

    game_object[0].position = position2d;    
    game_object[1].position = position2d + sf::Vector2f(size.x, 0);
    game_object[2].position = position2d + sf::Vector2f(size.x, size.y);
    game_object[3].position = position2d + sf::Vector2f(0, size.y);

    game_object[0].texCoords = sf::Vector2f(0, 0);
    game_object[1].texCoords = sf::Vector2f(size.x, 0);
    game_object[2].texCoords = sf::Vector2f(size.x, size.y);
    game_object[3].texCoords = sf::Vector2f(0, size.y);
    
    target.draw(game_object, states);
}

bool Animal::isSingleObject() const {
    return true;
}