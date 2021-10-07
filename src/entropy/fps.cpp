#include "fps.hpp"

using namespace entropy;

FPS::FPS() {
    this->m_frame = 0;
    this->m_fps   = 0;
}

FPS::~FPS() {}

// Update the FPS counter.
void FPS::update() {
    if(this->m_clock.getElapsedTime().asSeconds() >= 1.f) {
        this->m_fps   = this->m_frame;
        this->m_frame = 0;
        this->m_clock.restart();
    }

    ++this->m_frame;
}

// Get FPS.
unsigned int FPS::getFPS() {
    return this->m_fps;
}
