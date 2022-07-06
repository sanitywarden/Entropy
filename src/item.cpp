#include "item.hpp"

using namespace iso;

StorageItem::StorageItem() 
    : quantity(0), item_type(ItemType::TYPE_UNCATEGORISED), item_name("Item"), item_icon("icon_default")
{}

StorageItem::StorageItem(const std::string& name, int quantity, ItemType type)
    : quantity(quantity), item_type(type), item_name(name), item_icon("icon_default")
{}

StorageItem::StorageItem(const std::string& name, int quantity, ItemType type, const std::string& icon)
    : quantity(quantity), item_type(type), item_name(name), item_icon(icon)
{}

StorageItem::~StorageItem()
{}

StorageItem& StorageItem::operator+= (const StorageItem& item) {
    this->quantity += item.quantity;
    return *this;
}

StorageItem& StorageItem::operator-= (const StorageItem& item) {
    this->quantity -= item.quantity;
    return *this;
}

StorageItem StorageItem::operator+ (const StorageItem& item) {
    StorageItem temporary(this->item_name, this->quantity, this->item_type, this->item_icon);
    return temporary += item;
}

StorageItem StorageItem::operator- (const StorageItem& item) {
    StorageItem temporary(this->item_name, this->quantity, this->item_type, this->item_icon);
    return temporary -= item;
}

bool StorageItem::operator> (const StorageItem& item) const {
    return this->quantity > item.quantity;
}

bool StorageItem::operator>= (const StorageItem& item) const {
    return this->quantity >= item.quantity;
}

bool StorageItem::operator< (const StorageItem& item) const {
    return this->quantity < item.quantity;
}

bool StorageItem::operator<= (const StorageItem& item) const {
    return this->quantity >= item.quantity;
}