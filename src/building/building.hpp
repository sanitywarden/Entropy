#ifndef _BUILDING_HPP_
#define _BUILDING_HPP_

#include "gameObject.hpp"
#include "resources.hpp"
#include "generationSettings.hpp"

const auto VECTOR0X0         = sf::Vector2f(0, 0);
const auto VECTOR1X1         = sf::Vector2f(1, 1);
const auto VECTOR3X3         = sf::Vector2f(3, 3);
const auto BUILDINGSIZE64X64 = sf::Vector2f(64, 64);
const auto BUILDINGSIZE64X32 = sf::Vector2f(64, 32);

namespace iso {
    class Building : public GameObject {
        protected:
            int                numerical_type;
            sf::Vector2f       building_size;
            ResourceCollection building_cost;

            // Certain buildings can produce stuff, wood, stone or other items.
            // To produce a item, they need to be in proximity to a resource, and every instance of that resource increases the production rate.
            // This variable makes sure that in a certain area there can not be the same type of building (or rather, that the production efficiency will be lower).
            sf::Vector2f       building_proximity_area;
        
        public:
            GenerationSettings generation_settings;

        public:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            
            Building();
            Building(sf::Vector2f position, sf::Vector2f relative_position, sf::Vector2f size, std::string texture_name, std::string building_name, int numerical_type, sf::Vector2f building_size, sf::Vector2f proximity, ResourceCollection building_cost);
            Building(const Building& building);
            ~Building();

            /* Get the area occupied by the building. */
            const sf::Vector2f getBuildingArea() const;
            const sf::Vector2f getProductionArea() const;

            /* Get the cost of the building. */
            const ResourceCollection getBuildingCost() const;

            /* Get the refund of the building (when the building is destroyed). */
            const ResourceCollection getBuildingRefund() const;

            const int getNumericalType() const;

            void setGenerationSettings(const GenerationSettings& settings);

            // Assign ONLY BUILDING CLASS properties to this building.
            // GameObject properties are left untouched.
            void assignBuildingProperties(const Building& building);

            // Assign ALL properties to this building.
            // GameObject and Building properties are assigned.
            void assignAllProperties(const Building& building);

            // Equality and inequality operators return a value based on the building ID. 
            
            bool operator == (const Building& building);      
            bool operator != (const Building& building);

            // The argument type is GameObject instead of Region, because unfortunately, this project's include structure does not allow it.
            // If you would #include "region.hpp" here, then the application would not compile.
            // Although there is a workaround, because Region class inherits from GameObject, you can pass it as a parameter, and then
            // #include "region.hpp" in the implementation file.

            /* Implementation of the building's functionality. */
            virtual void update(GameObject* region, int building_index) { return; } 
    };
}

#endif