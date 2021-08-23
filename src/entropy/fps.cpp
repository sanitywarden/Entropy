#include "fps.hpp"

entropy::FPS::FPS() {
    this->m_frame = 0;
    this->m_fps = 0;
}

entropy::FPS::~FPS() {}

// Update the FPS counter.
void entropy::FPS::update() {
    if(this->m_clock.getElapsedTime().asSeconds() >= 1.f) {
        this->m_fps = this->m_frame;
        this->m_frame = 0;
        this->m_clock.restart();
    }

    ++this->m_frame;
}

// Get FPS.
const unsigned int& entropy::FPS::get() {
    return this->m_fps;
}