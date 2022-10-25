#pragma once

#include "abstractWidget.hpp"

#include <vector>

namespace gui {
class ImageList : public AbstractWidget {
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void createUI(const std::vector <std::string>& texture_list);
        WidgetData calculateItemData(const std::vector <std::string>& texture_list, int image_index);

    private:
        core::Vector2f corner_ratio;
        core::Vector2f image_ratio;

    public:
        ImageList(const WidgetData& data, core::Vector2f corner_ratio, core::Vector2f image_ratio);
        ~ImageList();

        void setImageList(const std::vector <std::string>& texture_list);
        void L_setImageList(luabridge::LuaRef reference);      
};

typedef std::shared_ptr <ImageList> ImageListComponent;
}