#include "engineSettings.hpp"

using namespace iso;

Settings::Settings() {
    this->window_size            = core::Vector2i(1200, 800);
    this->window_fullscreen      = false;
    this->window_vsync           = false;
    this->window_refresh_rate    = 144;
    this->application_debug_mode = true;
}

Settings::~Settings() {
}
