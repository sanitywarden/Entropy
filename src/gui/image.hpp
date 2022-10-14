#pragma once

#include "abstractWidget.hpp"

namespace gui {
class Image : public AbstractWidget {
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        std::string texture;
    
    public:
        Image(const WidgetData& data, const std::string& texture_id);
        ~Image();

        const std::string& getImageTexture() const;
        void setImageTexture(const std::string& texture_id);

        core::Vector2f getImageSize() const;
};

typedef std::shared_ptr <Image> ImageComponent;
}