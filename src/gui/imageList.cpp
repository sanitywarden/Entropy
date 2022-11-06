#include "imageList.hpp"
#include "globalutilities.hpp"
#include "simulationManager.hpp"
#include "image.hpp"
#include "interfacePage.hpp"

namespace gui {
ImageList::ImageList(const WidgetData& data, core::Vector2f corner_ratio, core::Vector2f image_ratio) 
    : AbstractWidget(data), corner_ratio(corner_ratio), image_ratio(image_ratio)
{}

ImageList::~ImageList()
{}

void ImageList::setImageList(const std::vector <std::string>& texture_list) {
    this->createUI(texture_list);
}

void ImageList::L_setImageList(luabridge::LuaRef reference) {
    if(reference.isNil()) {
        iso::printError("ImageList::L_setImageList()", "Table reference is nil");
        return;
    }

    if(!reference.length()) {
        iso::printWarning("ImageList::L_setImageList()", "Icon array is empty");
        return;
    }

    auto list = lua::readVectorString(reference);
    this->setImageList(list);
}

void ImageList::createUI(const std::vector <std::string>& texture_list) {
    if(!texture_list.size())
        return;

    this->data.components.clear();
    for(int i = 0; i < texture_list.size(); i++) {
        auto image_data = this->calculateItemData(texture_list, i);
        auto image = ImageComponent(new Image(image_data, texture_list.at(i)));
        this->addComponent(image);
    }
}

WidgetData ImageList::calculateItemData(const std::vector <std::string>& texture_list, int image_index) {
    auto widget_position = this->getParent()->getWidgetPosition();
    auto widget_size     = this->getParent()->getWidgetSize();
    auto image_size      = core::Vector2i(
        game_manager.resource.getTextureSize(texture_list.at(0)).x, 
        game_manager.resource.getTextureSize(texture_list.at(0)).y
    );
    
    auto distance_between_images = core::Vector2i(widget_size.x * this->image_ratio.x, widget_size.y * this->image_ratio.y);
    auto offset = core::Vector2i(widget_size.x * this->corner_ratio.x, widget_size.y * this->corner_ratio.y); // Offset of images from the border of underlying widget.

    int row = 0;
    for(const auto& texture : texture_list) {
        int a1 = widget_position.x + offset.x;
        int n  = row;
        int r  = image_size.x + distance_between_images.x;
        
        int lim = widget_position.x + widget_size.x - offset.x;
        int term = (n == 0)
            ? a1
            : a1 + (n - 1) * r;

        if(term + image_size.x < lim)
            row++;

        else
            break; 
    }

    if(row <= 0)
        iso::printError("ImageList::calculateItemData()", "Row size is not a positive integer: " + std::to_string(row));

    int divisor = (row - 1) == 0 ? row : row - 1;
    int space = (widget_size.x + -2 * offset.x + -row * image_size.x) / divisor;
    int x     = image_index % row;
    int y     = image_index / row;
    int ri    = image_index - (y * row); // Item index in the row.
    int hi    = row / 2;                 // Half items.
    int list_size = texture_list.size();

    core::Vector2i position;
    if(ri < hi || list_size < row || ri < row) {
        position =
        widget_position
        + offset
        + core::Vector2i(x * image_size.x, y * image_size.y)
        + core::Vector2i(x * space       , y * space);
    }

    else if(ri >= hi) {
        position = 
        widget_position
        + core::Vector2i(widget_size.x, 0)
        + core::Vector2i(-offset.x, offset.y)
        + core::Vector2i(-image_size.x, 0)
        + core::Vector2i(-(x - hi) * image_size.x, y * image_size.y)
        + core::Vector2i(-(x - hi) * space       , y * space);
    }

    // Row size is 2n + 1.
    // Middle item.
    else if(ri == hi + 1 && row % 2 == 1) {
        auto n_left  = this->calculateItemData(texture_list, image_index - 1);
        auto n_right = this->calculateItemData(texture_list, image_index + 1);

        auto middle_item_offset = (n_right.position.x - (n_left.position.x + image_size.x) - image_size.x) / 2;
        position = n_left.position + core::Vector2i(middle_item_offset, 0);
    }

    WidgetData data;
    data.position      = position;
    data.size          = image_size;
    data.widget_id     = "image_" + texture_list.at(image_index); 
    
    // Well, this is not true, since 'this' should be the parent. 
    // But it is difficult to assign the correct parent, so let it be.
    data.parent        = nullptr; 
    data.draw_priority = this->getDrawPriority() + 1;
    data.draw          = true;

    return data;
}

void ImageList::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for(auto& pair : this->data.components) {
        auto component = pair.second.get();
        target.draw(*component);
    }
}
}