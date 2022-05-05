#include "resources.hpp"

using namespace iso;

Resource::Resource()
    : quantity(0), name("*"), icon_name("*")
{}

Resource::Resource(std::string resource_name, int quantity)
    : quantity(quantity), name(resource_name), icon_name("*")
{}

Resource::Resource(std::string resource_name, std::string resource_icon, int quantity)
    : quantity(quantity), name(resource_name), icon_name(resource_icon)
{}

Resource::~Resource() {
    
}

Resource& Resource::operator+= (const Resource& cost) {
    this->quantity += cost.getQuantity();
    return *this;
}

Resource& Resource::operator-= (const Resource& cost) {
    this->quantity -= cost.getQuantity();
    return *this;
}

Resource Resource::operator+ (const Resource& cost) {
    Resource temporary(this->getName(), this->getIcon(), this->getQuantity());
    return temporary += cost;
}

Resource Resource::operator- (const Resource& cost) {
    Resource temporary(this->getName(), this->getIcon(), this->getQuantity());
    return temporary -= cost;
}

bool Resource::operator> (const Resource& cost) const {
    return this->quantity > cost.getQuantity();
}

bool Resource::operator>= (const Resource& cost) const {
    return this->quantity >= cost.getQuantity();
}

bool Resource::operator< (const Resource& cost) const {
    return this->quantity < cost.getQuantity();
}

bool Resource::operator<= (const Resource& cost) const {
    return this->quantity <= cost.getQuantity();
}

void Resource::setQuantity(int quantity) {
    this->quantity = quantity;
}

void Resource::incrementQuantity() {
    this->quantity++;
}

int Resource::getQuantity() const {
    return this->quantity;
}

std::string Resource::getName() const {
    return this->name;
}

std::string Resource::getIcon() const {
    return this->icon_name;
}
