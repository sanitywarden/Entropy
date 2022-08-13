#pragma once

#include "types.hpp"

namespace iso {
class ApplicationSettings {
    public:    
        core::Vector2i window_size;
        bool           window_fullscreen;
        bool           window_vsync;
        unsigned int   window_refresh_rate;
    public:
        ApplicationSettings();
        ~ApplicationSettings();
};
}