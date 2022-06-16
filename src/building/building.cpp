#include "building.hpp"
#include "region.hpp"
#include "globalutilities.hpp"

#include <iostream>
#include <fstream>

using namespace iso;

Building::Building() 
    : GameObject(VECTOR0X0X0, VECTOR0X0X0, VECTOR0X0, "*", "Building") 
{
    this->building_menu_icon      = "default";
    this->building_size           = VECTOR0X0;
    this->numerical_type          = 0;
    this->building_proximity_area = VECTOR0X0;
    this->building_name           = "Empty";
    this->removable               = true;
    this->building_cost           = RESOURCE_LOOKUP_TABLE; // All resource quantities are 0.
}

Building::Building(
    sf::Vector2f           size, 
    std::string            texture_name, 
    std::string            building_name, 
    std::string            building_menu_icon, 
    int                    numerical_type, 
    sf::Vector2f           building_size, 
    sf::Vector2f           proximity, 
    bool                   removable
) 
    : GameObject(VECTOR0X0X0, VECTOR0X0X0, size, texture_name, "Building") 
{
    this->building_menu_icon      = building_menu_icon;
    this->building_size           = building_size;
    this->numerical_type          = numerical_type;
    this->building_proximity_area = proximity;
    this->building_name           = building_name;
    this->removable               = removable;
    this->building_cost           = building_cost;
}

Building::Building(const Building& building) 
    : GameObject(building) 
{
    this->building_menu_icon      = building.getBuildingMenuIconName();
    this->building_size           = building.getBuildingArea();
    this->numerical_type          = building.getNumericalType();
    this->building_proximity_area = building.getProductionArea();
    this->building_name           = building.getBuildingName();
    this->removable               = building.isRemovable();
    this->building_cost           = building.getBuildingCost();
}

Building::~Building() {
    
}

void Building::loadResourceCost() {
    const std::string filename = "./config/building_resource_cost.config";
    std::fstream config_file(filename);

    if(!config_file.good()) {
        std::cout << "[Building]: Could not open resource config file for " << this->getBuildingName() << ".\n";
        this->building_cost = RESOURCE_LOOKUP_TABLE;
        return;
    }

    std::string line_delimiter  = ",";   // What char marks that a line ends.
    std::string read_value_from = ":";   // What char marks that value is afterwards.
    char comment_indicator = '#';        // What char marks a comment.
    int  ascii_empty_line_indicator = 0; // What value marks that a line is empty (ASCII NULL).

    std::string line_content;
    std::vector <Resource> cost;
    while(std::getline(config_file, line_content)) {
        if(line_content[0] == comment_indicator || (int)line_content[0] == ascii_empty_line_indicator)
            continue;

        auto index = find(line_content, read_value_from);
        std::string property_name  = readBefore(line_content, read_value_from);  
        std::string property_value = read(line_content, index + 1, line_content.length() - 1);
        std::string building_name_uppercase = toHigher(this->getTextureName());  

        if(startsWith(property_name, building_name_uppercase)) {
            std::string resource_name = capitalise(readAfter(property_name, (building_name_uppercase + "_")));
            int resource_quantity     = std::stoi(property_value);

            Resource resource(resource_name, resource_quantity);
            cost.push_back(resource);
        }
    }

    this->building_cost = cost;
}

const sf::Vector2f Building::getBuildingArea() const {
    return this->building_size;
}

const sf::Vector2f Building::getProductionArea() const {
    return this->building_proximity_area;
}

const std::vector <Resource> Building::getBuildingCost() const {
    return this->building_cost;
}

const std::vector <Resource> Building::getBuildingRefund() const {
    std::vector <Resource> refund;
    for(const auto& resource : this->building_cost) {
        Resource refund_resource(resource.getName(), resource.getIcon(), resource.getType(), resource.getQuantity() * 0.90f);
        refund.push_back(refund_resource);
    }

    return refund;
}

const int Building::getNumericalType() const {
    return this->numerical_type;
}

bool Building::operator== (const Building& building) const {
    return this->numerical_type == building.getNumericalType();
}

bool Building::operator!= (const Building& building) const {
    return !(*this == building);  
}

void Building::draw(sf::RenderTarget& target, sf::RenderStates states) const {
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

void Building::assignBuildingProperties(const Building& building) {
    this->numerical_type = building.numerical_type;
    this->building_size  = building.building_size;
    this->building_name  = building.building_name;
    this->removable      = building.removable;
}

void Building::assignAllProperties(const Building& building) {
    this->assignBuildingProperties(building);

    this->object_name         = building.getName();
    this->object_position     = building.getPosition();
    this->object_size         = building.getSize();
    this->object_texture_name = building.getTextureName();
    this->object_colour       = building.getColour();
}

std::string Building::getBuildingMenuIconName() const {
    return this->building_menu_icon;
}

std::string Building::getBuildingName() const {
    return this->building_name;
}

void Building::setBuildingName(std::string name) {
    this->building_name = name;
}

bool Building::isRemovable() const {
    return this->removable;
}