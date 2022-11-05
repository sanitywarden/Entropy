#pragma once

#include "types.hpp"

#include "building.hpp"
#include "player.hpp"
#include "gamestate.hpp"
#include "region.hpp"
#include "worldmap.hpp"
#include "regionmap.hpp"

#include "gui/label.hpp"
#include "gui/imageList.hpp"

namespace lua {
void registerLua();

// Resources

void L_loadTexture(const std::string& filename, const std::string& id, core::Vector2i position, core::Vector2i size);
void L_loadFont(const std::string& filename, const std::string& id);
void L_loadEvent(const std::string& id, float required_time, bool speed_dependant);
void L_createIcon(const std::string& id);

// Gamestate

void L_setGamestate(const std::string& id);
iso::Gamestate* L_getCurrentGamestate();
iso::Gamestate* L_getGamestate(const std::string& state_id);
iso::Regionmap* L_getRegionmap();
iso::Worldmap* L_getWorldmap();
bool L_mouseIntersectsUI();
std::string L_getInterfacePageUnderMouse();
std::string L_getInterfaceItemUnderMouse(const std::string& page_id);
void L_showInterface(const std::string& id);
void L_hideInterface(const std::string& id);
bool L_isInterfaceVisible(const std::string& id);
core::Vector2i L_getMousePosition();
core::Vector2i L_getMousePositionInterface();

// Engine

void L_exitApplication(int code);
int L_getFPS();
float L_getFrameTime();

// Window
core::Vector2i L_getWindowSize();
int L_getWindowHeight();
int L_getWindowWidth();
bool L_inScreenSpace(const iso::GameObject& gameobject);
void L_drawGameObject(iso::GameObject& gameobject);
void L_drawBuilding(iso::Building& building);
void L_setVSync(bool on);
void L_setMaxFramerate(int framerate);
void L_takeScreenshot();

// Controls

bool L_isKeyPressed(const std::string& key_id);
bool L_isLeftMouseButtonPressed();
bool L_isRightMouseButtonPressed();
bool L_isMiddleMouseButtonPressed();
bool L_isInputBlocked();
void L_blockInput(bool block);

// GUI

gui::Label* L_getComponentLabel(const std::string& page_id, const std::string& label_id);
gui::ImageList* L_getComponentImageList(const std::string& page_id, const std::string& list_id);

// Miscellaneous    

int L_getRegionIndex();
int L_getTileIndex();
iso::Region* L_getRegion(int region_index);
bool L_isBuildingTile(int tile_index);
bool L_inWorldBounds(int region_index);
bool L_inRegionBounds(int tile_index);
core::Vector2i L_tileGridPosition(int tile_index);
void L_tickProduction(int region_index, int tile_index);
void L_tickHarvest(int region_index, int tile_index);

// World generation

void L_generateRegion(int region_index);
void L_generateWorld();

// Player

iso::Player* L_getHumanPlayer();
iso::Player* L_getPlayer(int player_id);
bool L_isHumanPlayer(int player_id);

/*
bool L_canRegionBeColonised(int region_index);
bool L_canAffordBuilding(int player_id, const Building& building);
bool L_isRegionOwned(int region_index);
std::vector <int> L_astar(int start_index, int end_index);
Player* L_getRegionOwner(int region_index);
*/

}