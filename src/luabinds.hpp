#pragma once

#include "types.hpp"
#include "gui/label.hpp"

namespace lua {
void registerLua();

void L_loadTexture(const std::string& filename, const std::string& id, core::Vector2i position, core::Vector2i size);
void L_loadFont(const std::string& filename, const std::string& id);
void L_loadEvent(const std::string& id, float required_time, bool speed_dependant);
void L_createIcon(const std::string& id);

void L_setGamestate(const std::string& id);
void L_showInterface(const std::string& id);
void L_hideInterface(const std::string& id);
bool L_isInterfaceVisible(const std::string& id);

int L_getFPS();
float L_getFrameTime();

bool L_isKeyPressed(const std::string& key_id);
gui::Label* L_getComponentLabel(const std::string& page_id, const std::string& label_id);
}