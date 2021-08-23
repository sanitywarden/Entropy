#include "windowManager.hpp"

#include <iostream>

entropy::windowManager::windowManager() {}
entropy::windowManager::~windowManager() {}

void entropy::windowManager::createFullscreenWindow() {
    std::cout << "[Entropy Engine][Window Manager]: Window created in fullscreen mode.\n";
    
    this->m_window = std::shared_ptr <sf::RenderWindow> (new sf::RenderWindow(sf::VideoMode::getFullscreenModes()[0], "", sf::Style::Fullscreen));
}

void entropy::windowManager::createWindow(const sf::Vector2f& window_size) {
    std::cout << "[Entropy Engine][Window Manager]: Window created in custom size mode.\n";

    this->m_window = std::shared_ptr <sf::RenderWindow> (new sf::RenderWindow(sf::VideoMode(window_size.x, window_size.y), ""));
}

sf::RenderWindow* entropy::windowManager::getWindow() {
    return this->m_window.get();
}

sf::Vector2f entropy::windowManager::getWindowSize() {
    auto size = sf::Vector2f(
        this->getWindow()->getSize().x,
        this->getWindow()->getSize().y
    );

    return size;
}

void entropy::windowManager::clear(const sf::Color& colour) {
    this->m_window.get()->clear(colour);
}

void entropy::windowManager::display() {
    this->m_window.get()->display();
}