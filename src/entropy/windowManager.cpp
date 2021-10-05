#include "windowManager.hpp"

#include <iostream>

using namespace entropy;

windowManager::windowManager() {}
windowManager::~windowManager() {}

void windowManager::createFullscreenWindow() {
    std::cout << "[Entropy Engine][Window Manager]: Window created in fullscreen mode.\n";
    
    this->m_window = std::shared_ptr <sf::RenderWindow> (new sf::RenderWindow(sf::VideoMode::getFullscreenModes()[0], "", sf::Style::Fullscreen));
}

void windowManager::createWindow(const sf::Vector2f& window_size) {
    std::cout << "[Entropy Engine][Window Manager]: Window created in custom size mode.\n";

    this->m_window = std::shared_ptr <sf::RenderWindow> (new sf::RenderWindow(sf::VideoMode(window_size.x, window_size.y), ""));
}

sf::RenderWindow* windowManager::getWindow() {
    return this->m_window.get();
}

sf::Vector2f windowManager::getWindowSize() {
    auto size = sf::Vector2f(
        this->getWindow()->getSize().x,
        this->getWindow()->getSize().y
    );

    return size;
}

bool windowManager::open() {
    return this->getWindow()->isOpen();
}

void windowManager::setTitle(std::string title) {
    this->getWindow()->setTitle(title);
}

void windowManager::clear(const sf::Color& colour) {
    this->getWindow()->clear(colour);
}

void windowManager::display() {
    this->getWindow()->display();
}