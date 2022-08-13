#include "windowManager.hpp"
#include <iostream>

using namespace iso;

WindowManager::WindowManager() 
    : window(nullptr)
{}

WindowManager::~WindowManager() 
{}

void WindowManager::createFullscreenWindow() {
    auto desktop_size = sf::VideoMode(
        sf::VideoMode::getDesktopMode().width,
        sf::VideoMode::getDesktopMode().height
    );
    
    this->window = std::make_shared <sf::RenderWindow> (sf::VideoMode(desktop_size), "", sf::Style::Fullscreen);
}

void WindowManager::createWindow(const ApplicationSettings& settings, const std::string& title) {
    if(this->window != nullptr)
        return;

    if(settings.window_fullscreen)
        this->createFullscreenWindow();

    else
        this->window = std::make_shared <sf::RenderWindow> (sf::VideoMode(settings.window_size.x, settings.window_size.y), "");

    this->setMaxFramerate(settings.window_refresh_rate);
    this->setVsync(settings.window_vsync);
    this->setTitle(title);
}

sf::RenderWindow* WindowManager::getWindow() {
    return this->window.get();
}

sf::Vector2f WindowManager::getWindowSize() const {
    return sf::Vector2f(
        this->window.get()->getSize().x,
        this->window.get()->getSize().y
    );
}

unsigned int WindowManager::getWindowWidth() const {
    return this->window.get()->getSize().x;
}

unsigned int WindowManager::getWindowHeight() const {
    return this->window.get()->getSize().y;
}

bool WindowManager::isOpen() const {
    return this->window.get()->isOpen();
}

bool WindowManager::isFocused() const {
    return this->window.get()->hasFocus();
}

void WindowManager::setTitle(const std::string& title) {
    this->window->setTitle(title);
}

void WindowManager::clear(const sf::Color& clear_colour) {
    this->window->clear(clear_colour);
}

void WindowManager::display() {
    this->window.get()->display();
}

void WindowManager::draw(const sf::Drawable& drawable, sf::RenderStates states) {
    this->window.get()->draw(drawable, states);
}

void WindowManager::close() {
   this->window.get()->close();
}

void WindowManager::setVsync(bool on) {
    this->window.get()->setVerticalSyncEnabled(on);
}

void WindowManager::setKeyHold(bool on) {
    this->window.get()->setKeyRepeatEnabled(on);
}

void WindowManager::setMaxFramerate(unsigned int framerate) {
    this->window.get()->setFramerateLimit(framerate);
}

void WindowManager::takeScreenshot() const {
    auto screenshot_time = std::to_string(time(0));
    
    sf::Texture texture;
    texture.create(this->getWindowSize().x, this->getWindowSize().y);
    texture.update(*this->window.get());
    auto screenshot = texture.copyToImage();
    screenshot.saveToFile("./screenshot/screenshot_" + screenshot_time + ".png");  
}

void WindowManager::setView(const sf::View& view) {
    this->window.get()->setView(view);
}

const sf::View& WindowManager::getView() const {
    return this->window.get()->getView();
}