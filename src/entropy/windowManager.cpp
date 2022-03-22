#include "windowManager.hpp"
#include <iostream>

using namespace entropy;

windowManager::windowManager() {

}

windowManager::windowManager(const Settings& settings) {
    this->m_settings = settings;
}

windowManager::~windowManager() {

}

void windowManager::createFullscreenWindow() {
    if(this->m_settings.application_debug_mode)
        std::cout << "[Entropy Engine][Window Manager]: Window created in fullscreen mode.\n";
    
    this->m_window = std::shared_ptr <sf::RenderWindow> (new sf::RenderWindow(sf::VideoMode::getFullscreenModes()[0], "", sf::Style::Fullscreen));
}

void windowManager::createWindow(sf::Vector2f window_size) {
    if(this->m_settings.application_debug_mode)
        std::cout << "[Entropy Engine][Window Manager]: Window created in custom size mode.\n";

    this->m_window = std::shared_ptr <sf::RenderWindow> (new sf::RenderWindow(sf::VideoMode(window_size.x, window_size.y), ""));
}

sf::RenderWindow* windowManager::getWindow() {
    return this->m_window.get();
}

sf::Vector2f windowManager::windowSize() {
    return sf::Vector2f(
        this->getWindow()->getSize().x,
        this->getWindow()->getSize().y
    );
}

unsigned int windowManager::windowWidth() {
    return this->getWindow()->getSize().x;
}

unsigned int windowManager::windowHeight() {
    return this->getWindow()->getSize().y;
}

bool windowManager::open() {
    return this->getWindow()->isOpen();
}

void windowManager::setTitle(std::string title) {
    this->getWindow()->setTitle(title);
}

void windowManager::clear(const sf::Color& clear_colour) {
    this->getWindow()->clear(clear_colour);
}

void windowManager::display() {
    this->getWindow()->display();
}

void windowManager::draw(const sf::Drawable& drawable, sf::RenderStates states) {
    this->getWindow()->draw(drawable, states);
}

void windowManager::close() {
   this->getWindow()->close();
}

void windowManager::setVsync(bool on) {
    this->getWindow()->setVerticalSyncEnabled(on);
}

void windowManager::setKeyHold(bool on) {
    this->getWindow()->setKeyRepeatEnabled(on);
}

void windowManager::setMaxFramerate(unsigned int framerate) {
    this->getWindow()->setFramerateLimit(framerate);
}

bool windowManager::focused() {
    return this->getWindow()->hasFocus();
}