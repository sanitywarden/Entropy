#include "minimap.hpp"
#include "generationSettings.hpp"
#include "regionmap.hpp"
#include "colours.hpp"

#include <iostream>

using namespace gui;
using namespace iso;

WidgetMinimap::WidgetMinimap() 
    : InterfacePage(nullptr)
{
    std::cout << "[GUI][WidgetMinimap]: Simulation manager is a nullptr.\n";
}

WidgetMinimap::WidgetMinimap(SimulationManager* manager) 
    : InterfacePage(manager) 
{
    this->setWidgetID("component_minimap");
    this->createUI();   

    this->should_redraw = true; 
}

WidgetMinimap::~WidgetMinimap() {

}

void WidgetMinimap::createUI() {
    auto widget_size = sf::Vector2f(256, 256);
    this->minimap_image.create(widget_size.x, widget_size.y);
}

void WidgetMinimap::updateUI() {
    if(!this->manager)
        return;

    if(this->should_redraw) {
        auto regionmap = static_cast<Regionmap*>(this->manager->gamestate.getGamestate());
        auto* current_region = regionmap->getCurrentRegion();

        auto image_size = this->minimap_image.getSize();
        const int scale_difference = image_size.x / game_settings.getRegionWidth(); 

        for(int y = 0; y < image_size.y; y++) {
            for(int x = 0; x < image_size.x; x++) {
                const int scaled_x = x / scale_difference;
                const int scaled_y = y / scale_difference;
                
                const int index  = game_settings.calculateRegionIndex(scaled_x, scaled_y);
                const auto& tile = current_region->map[index]; 

                // Set pixel colours.
                if(tile.tiletype.is_water())
                    this->minimap_image.setPixel(x, y, COLOUR_BLUE);
                
                else if(current_region->treeExistsAt(index))
                    this->minimap_image.setPixel(x, y, COLOUR_GREEN);

                else if(tile.getTextureName() == "tile_resource_flint")
                    this->minimap_image.setPixel(x, y, COLOUR_GREY_FLINT);
            
                else if(tile.getTextureName() == "tile_resource_stone")
                    this->minimap_image.setPixel(x, y, COLOUR_GREY_STONE);

                // A tile.
                else
                    this->minimap_image.setPixel(x, y, current_region->biome.getBiomeWorldmapColour());
            }
        }
        
        sf::Texture texture;
        texture.loadFromImage(this->minimap_image);
        this->manager->resource.addTexture("region_minimap", texture, sf::IntRect(0, 0, image_size.x, image_size.y));

        this->should_redraw = false;
    }
}

void WidgetMinimap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(!this->manager)
        return;

    if(!this->isVisible())
        return;

    int window_width  = this->manager->window.windowWidth();
    int window_height = this->manager->window.windowHeight(); 

    auto widget_size     = this->minimap_image.getSize();
    auto widget_position = sf::Vector2f(window_width - widget_size.x, 0);

    sf::VertexArray image(sf::Quads, 4);
    
    image[0].position = widget_position;
    image[1].position = widget_position + sf::Vector2f(widget_size.x, 0);
    image[2].position = widget_position + sf::Vector2f(widget_size.x, widget_size.y);
    image[3].position = widget_position + sf::Vector2f(0, widget_size.y);

    image[0].texCoords = sf::Vector2f(0, 0);
    image[1].texCoords = sf::Vector2f(widget_size.x, 0);
    image[2].texCoords = sf::Vector2f(widget_size.x, widget_size.y);
    image[3].texCoords = sf::Vector2f(0, widget_size.y);

    states.texture = &this->manager->resource.getTexture("region_minimap");
    target.draw(image, states);
}