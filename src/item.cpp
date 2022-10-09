#include "item.hpp"
#include "resource.hpp"

using namespace iso;

StorageItem::StorageItem() 
{}

StorageItem::StorageItem(const StorageItem& item)
    : data(item.data)
{}

StorageItem::StorageItem(const ItemData& data)
    : data(data)
{}

StorageItem::~StorageItem()
{}

StorageItem& StorageItem::operator+= (const StorageItem& item) {
    this->data.amount += item.data.amount;
    return *this;
}

StorageItem& StorageItem::operator-= (const StorageItem& item) {
    this->data.amount -= item.data.amount;
    return *this;
}

StorageItem StorageItem::operator+ (const StorageItem& item) {
    StorageItem temp(item);
    return temp += item;
}

StorageItem StorageItem::operator- (const StorageItem& item) {
    StorageItem temp(item);
    return temp -= item;
}

bool StorageItem::operator> (const StorageItem& item) const {
    return this->data.amount > item.data.amount;
}

bool StorageItem::operator>= (const StorageItem& item) const {
    return this->data.amount >= item.data.amount;
}

bool StorageItem::operator< (const StorageItem& item) const {
    return this->data.amount < item.data.amount;
}

bool StorageItem::operator<= (const StorageItem& item) const {
    return this->data.amount <= item.data.amount;
}

StorageItem& StorageItem::operator= (const Resource& resource) {
    this->data.name         = resource.getResourceName();
    this->data.description  = resource.getResourceDescription();
    this->data.icon_size    = core::Vector2i(resource.getIconTextureSize().x, resource.getIconTextureSize().y);
    this->data.icon_texture = resource.getIconTexture();
    this->data.type         = resource.getResourceType();
    return *this;
}

const std::string& StorageItem::getDefinitionFilename() const {
    return this->data.filename;
}

const std::string& StorageItem::getItemName() const {
    return this->data.name;
}

const std::string& StorageItem::getItemDescription() const {
    return this->data.description;
}

const std::string& StorageItem::getIconTexture() const {
    return this->data.icon_texture;
}

const core::Vector2i StorageItem::getIconTextureSize() const {
    return this->data.icon_size;
}

const std::string& StorageItem::getItemType() const {
    return this->data.type;
}

int StorageItem::getAmount() const {
    return this->data.amount;
}

void StorageItem::setAmount(int amount) {
    this->data.amount = amount;
}