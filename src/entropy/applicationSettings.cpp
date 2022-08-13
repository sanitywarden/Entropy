#include "applicationSettings.hpp"

using namespace iso;

ApplicationSettings::ApplicationSettings()
    : window_size(core::Vector2i(1200, 800)), window_fullscreen(false), window_vsync(false), window_refresh_rate(144)
{}

ApplicationSettings::~ApplicationSettings() 
{}
