#include "luabinds.hpp"
#include "simulationManager.hpp"
#include "globalutilities.hpp"
#include "worldData.hpp"

#include <map>
#include <LuaBridge/Vector.h>
#include <LuaBridge/Map.h>

using namespace iso;

namespace lua {
void registerLua() {
    std::cout << "[] Binding C++ to Lua.\n";

    luabridge::getGlobalNamespace(game_manager.lua())
        .beginClass <core::Colour> ("Colour")
            .addConstructor <void (*) (uint8_t, uint8_t, uint8_t, uint8_t)> ()
            .addProperty("r", core::Colour::getR, core::Colour::setR)
            .addProperty("g", core::Colour::getG, core::Colour::setG)
            .addProperty("b", core::Colour::getB, core::Colour::setB)
            .addProperty("a", core::Colour::getA, core::Colour::setA)
        .endClass()
        .beginClass <core::Vector2f> ("Vector2f")
            .addConstructor <void (*) (float, float)> ()
            .addProperty("x", core::Vector2f::getX, core::Vector2f::setX) 
            .addProperty("y", core::Vector2f::getY, core::Vector2f::setY) 
        .endClass()
        .beginClass <core::Vector2i> ("Vector2i")
            .addConstructor <void (*) (float, float)> ()
            .addProperty("x", core::Vector2i::getX, core::Vector2i::setX) 
            .addProperty("y", core::Vector2i::getY, core::Vector2i::setY) 
        .endClass()
        .beginClass <core::Vector3f> ("Vector3f")
            .addConstructor <void (*) (float, float, float)> ()
            .addProperty("x", core::Vector3f::getX, core::Vector3f::setX) 
            .addProperty("y", core::Vector3f::getY, core::Vector3f::setY) 
            .addProperty("z", core::Vector3f::getZ, core::Vector3f::setZ) 
        .endClass()
        .beginClass <core::Vector3i> ("Vector3i")
            .addConstructor <void (*) (int, int, int)> ()
            .addProperty("x", core::Vector3i::getX, core::Vector3i::setX) 
            .addProperty("y", core::Vector3i::getY, core::Vector3i::setY) 
            .addProperty("z", core::Vector3i::getZ, core::Vector3i::setZ) 
        .endClass()
        .beginClass <core::Ratio> ("Ratio")
            .addConstructor <void (*) (std::string, std::string)> ()
        .endClass()
        .beginClass <iso::GameObject> ("GameObject")
            .addConstructor <void (*) (core::Vector3i, core::Vector2i, const std::string&)> ()
            .addProperty("object_name"       , iso::GameObject::getName       , iso::GameObject::setName)
            .addProperty("object_position"   , iso::GameObject::getPosition   , iso::GameObject::setPosition)
            .addProperty("object_size"       , iso::GameObject::getSize       , iso::GameObject::setSize)
            .addProperty("object_texture"    , iso::GameObject::getTextureName, iso::GameObject::setTextureName)
            .addProperty("object_colour"     , iso::GameObject::getColour     , iso::GameObject::setColour)
            .addProperty("object_instance_id", iso::GameObject::getInstanceId)
            .addProperty("object_position2d" , iso::GameObject::getPosition2D)
            .addProperty("index", iso::GameObject::getIndex)
            .addFunction("exists"     , iso::GameObject::exists)
            .addFunction("contains"   , iso::GameObject::contains)
            .addFunction("hasName"    , iso::GameObject::hasName)
        .endClass()
        .beginClass <Gamestate> ("Gamestate")
            .addFunction("getID", Gamestate::getStateId)
        .endClass()
        .beginClass <Worldmap> ("Worldmap")
            .addProperty("current_index" , Worldmap::L_getCurrentIndex)
            .addProperty("selected_index", Worldmap::L_getSelectedIndex, Worldmap::L_setSelectedIndex)
        .endClass()
        .beginClass <Regionmap> ("Regionmap")
            .addProperty("current_tile_index", Regionmap::L_getTileIndex)
            .addProperty("region_index"      , Regionmap::L_getRegionIndex, Regionmap::L_setRegionIndex)
        .endClass()
        .deriveClass <Building, GameObject> ("Building")
            .addProperty("name"       , Building::getBuildingName       , Building::setBuildingName)
            .addProperty("description", Building::getBuildingDescription, Building::setBuildingDescription)
            .addProperty("area"       , Building::getBuildingArea       , Building::setBuildingArea)
            .addProperty("texture"    , Building::getBuildingTexture    , Building::setBuildingTexture)
            .addProperty("scan_area"  , Building::getBuildingScanArea   , Building::setBuildingScanArea)
            .addProperty("bcost"      , Building::getBuildingCost       , Building::setBuildingCost)
            .addProperty("icon"       , Building::getBuildingIcon       , Building::setBuildingIcon)
            .addProperty("icon_size"  , Building::getBuildingIconSize   , Building::setBuildingIconSize)
            .addProperty("harvest"    , Building::getBuildingHarvests   , Building::setBuildingHarvests)
            .addProperty("production" , Building::getBuildingProduction , Building::setBuildingProduction)
            .addProperty("removable"  , Building::isRemovable           , Building::setRemovable)
            .addFunction("isProductionBuilding", Building::isProductionBuilding)
            .addFunction("isHarvestBuilding"   , Building::isHarvestBuilding)
        .endClass()
        .beginClass <Biome> ("Biome")
            .addProperty("name"               , Biome::getBiomeName            , Biome::setBiomeName)
            .addProperty("id"                 , Biome::getBiomeId              , Biome::setBiomeId)
            .addProperty("description"        , Biome::getBiomeDescription     , Biome::setBiomeDescription)
            .addProperty("wmap_colour"        , Biome::getBiomeWorldmapColour  , Biome::setBiomeWorldmapColour)
            .addProperty("temperature"        , Biome::getBiomeTemperature     , Biome::setBiomeTemperature)
            .addProperty("moisture"           , Biome::getBiomeMoisture        , Biome::setBiomeMoisture)
            .addProperty("wmap_forest_texture", Biome::getWorldmapForestTexture, Biome::setWorldmapForestTexture)
            .addProperty("tile_list"          , Biome::getBiomeTileList        , Biome::setBiomeTileList)
            .addProperty("tree_list"          , Biome::getBiomeTreeList        , Biome::setBiomeTreeList)
        .endClass()
        .beginClass <Resource> ("Resource")
            .addProperty("name"       , Resource::getResourceName       , Resource::setResourceName)
            .addProperty("description", Resource::getResourceDescription, Resource::setResourceDescription)
            .addProperty("texture"    , Resource::getResourceTexture    , Resource::setResourceTexture)
            .addProperty("icon"       , Resource::getIconTexture        , Resource::setResourceIcon)
            .addProperty("type"       , Resource::getResourceType       , Resource::setResourceType)
            .addFunction("asItem"     , Resource::asItem)
        .endClass()
        .beginClass <StorageItem> ("Item")
            .addProperty("name"       , StorageItem::getItemName)
            .addProperty("description", StorageItem::getItemDescription)
            .addProperty("icon"       , StorageItem::getIconTexture)
            .addProperty("type"       , StorageItem::getItemType)
            .addProperty("amount"     , StorageItem::getAmount)
        .endClass()
        .deriveClass <Tile, GameObject> ("Tile")
            .addFunction("isTerrain", Tile::isTerrain)
            .addFunction("isWater"  , Tile::isWater)
            .addFunction("isOcean"  , Tile::isOcean)
            .addFunction("isRiver"  , Tile::isRiver)
            .addFunction("isCoast"  , Tile::isCoast)
        .endClass()
        .deriveClass <Region, GameObject> ("Region")
            .addFunction("hasBiome"               , Region::L_hasBiome)
            .addFunction("getBiome"               , Region::L_getBiome)
            .addFunction("isTerrain"              , Region::L_isTerrain)
            .addFunction("isOcean"                , Region::L_isOcean)
            .addFunction("isRiver"                , Region::L_isRiver)
            .addFunction("isCoast"                , Region::L_isCoast)
            .addFunction("isLake"                 , Region::L_isLake)
            .addFunction("isWater"                , Region::L_isWater)
            .addFunction("isForest"               , Region::L_isForest)
            .addFunction("getTileAt"              , Region::L_getTileAt)
            .addFunction("isTileOccupied"         , Region::L_isTileOccupied)
            .addFunction("isBuildingPositionValid", Region::L_isBuildingPositionValid)
            .addFunction("treeExistsAt"           , Region::L_treeExistsAt)
            .addFunction("getTreeAt"              , Region::L_getTreeAt)
            .addFunction("buildingExistsAt"       , Region::L_buildingExistsAt)
            .addFunction("getBuildingAt"          , Region::L_getBuildingAt)
            .addFunction("resourceExistsAt"       , Region::L_resourceExistsAt)
            .addFunction("getResourceAt"          , Region::L_getResourceAt)
            .addFunction("getMoistureString"      , Region::L_getMoistureString)
            .addFunction("getMoistureNumber"      , Region::L_getMoistureNumber)
            .addFunction("getTemperatureString"   , Region::L_getTemperatureString)
            .addFunction("getTemperatureNumber"   , Region::L_getTemperatureNumber)
            .addFunction("constructBuilding"      , Region::L_constructBuilding)
            .addFunction("demolishBuilding"       , Region::L_demolishBuilding)
            .addFunction("hasOwner"               , Region::L_hasOwner)
            .addFunction("getOwnerId"             , Region::L_getOwnerId)
            .addFunction("stockpileAdd"           , Region::L_stockpileAdd)
            .addFunction("stockpileRemove"        , Region::L_stockpileRemove)
            .addFunction("inStockpile"            , Region::L_inStockpile)
            .addFunction("getBuildingList"        , Region::L_getBuildingList)
            .addFunction("getStockpile"           , Region::L_getStockpile)
        .endClass()
        .beginClass <Player> ("Player") 
            .addProperty("id"        , Player::getID)
            .addProperty("is_human"  , Player::isHuman)
            .addProperty("capital"   , Player::getCapital)
            .addProperty("origin"    , Player::getInitialSpawn)
            .addProperty("country"   , Player::getCountryName)
            .addProperty("culture"   , Player::getCultureGroup)
            .addProperty("colour"    , Player::getCountryColourFull)
            .addProperty("map_colour", Player::getCountryColourTransparent)            
            .addFunction("canAffordBuilding"  , Player::canAffordBuilding)
            .addFunction("hasDiscoveredRegion", Player::discoveredRegion)
            .addFunction("addKnownRegion"     , Player::addKnownRegion)
        .endClass()
        .beginClass <gui::AbstractWidget> ("AbstractWidget")
            .addProperty("id"              , gui::AbstractWidget::getWidgetID)
            .addProperty("position"        , gui::AbstractWidget::getWidgetPosition)
            .addProperty("size"            , gui::AbstractWidget::getWidgetSize)
            .addProperty("draw_priority"   , gui::AbstractWidget::getDrawPriority)
            .addProperty("should_draw"     , gui::AbstractWidget::isVisible, gui::AbstractWidget::setVisible)
            .addFunction("hasParent"       , gui::AbstractWidget::hasParent)
            .addFunction("getParent"       , gui::AbstractWidget::getParent)
            .addFunction("hasChildren"     , gui::AbstractWidget::hasChildren)
            .addFunction("getChildren"     , gui::AbstractWidget::getChildren)
            .addFunction("hasEventOverride", gui::AbstractWidget::hasEventOverride)
            .addFunction("containsPoint"   , gui::AbstractWidget::containsPoint)
        .endClass()
        .deriveClass <gui::Label, gui::AbstractWidget> ("Label")
            .addProperty("text"  , gui::Label::getString, gui::Label::setString)
            .addProperty("colour", gui::Label::getColour, gui::Label::setColour)
            .addProperty("font"  , gui::Label::getFont  , gui::Label::setFont)
        .endClass()
        .deriveClass <gui::ImageList, gui::AbstractWidget> ("ImageList")
            .addFunction("setImageList", gui::ImageList::L_setImageList)
        .endClass()
        .deriveClass <gui::InterfacePage, gui::AbstractWidget> ("InterfacePage")

        .endClass()
        
        // Resources
        .addFunction("loadTexture"               , &lua::L_loadTexture)
        .addFunction("loadFont"                  , &lua::L_loadFont)
        .addFunction("loadScheduledEvent"        , &lua::L_loadEvent)
        .addFunction("createIcon"                , &lua::L_createIcon)
        
        // Gamestate
        .addFunction("setGamestate"              , &lua::L_setGamestate)
        .addFunction("getCurrentGamestate"       , &lua::L_getCurrentGamestate)
        .addFunction("getGamestate"              , &lua::L_getGamestate)
        .addFunction("getWorldmap"               , &lua::L_getWorldmap)
        .addFunction("getRegionmap"              , &lua::L_getRegionmap)
        .addFunction("mouseIntersectsUI"         , &lua::L_mouseIntersectsUI)
        .addFunction("getInterfaceUnderMouse"    , &lua::L_getInterfacePageUnderMouse)
        .addFunction("getInterfaceItemUnderMouse", &lua::L_getInterfaceItemUnderMouse)
        .addFunction("showInterface"             , &lua::L_showInterface)
        .addFunction("hideInterface"             , &lua::L_hideInterface)
        .addFunction("isInterfaceVisible"        , &lua::L_isInterfaceVisible)
        .addFunction("getMousePosition"          , &lua::L_getMousePosition)
        .addFunction("getMousePositionInterface" , &lua::L_getMousePositionInterface)

        // Engine
        .addFunction("getFPS"                    , &lua::L_getFPS)
        .addFunction("getFrameTime"              , &lua::L_getFrameTime)
        .addFunction("exitApplication"           , &lua::L_exitApplication)
        
        // Window
        .addFunction("getWindowSize"   , &lua::L_getWindowSize)
        .addFunction("getWindowHeight" , &lua::L_getWindowHeight)
        .addFunction("getWindowWidth"  , &lua::L_getWindowWidth)
        .addFunction("inScreenSpace"   , &lua::L_inScreenSpace)
        .addFunction("drawGameObject"  , &lua::L_drawGameObject)
        .addFunction("drawBuilding"    , &lua::L_drawBuilding)
        .addFunction("setVSync"        , &lua::L_setVSync)
        .addFunction("setMaxFramerate" , &lua::L_setMaxFramerate)
        .addFunction("takeScreenshot"  , &lua::L_takeScreenshot)

        // Controls
        .addFunction("isKeyPressed"              , &lua::L_isKeyPressed)
        .addFunction("isLeftMouseButtonPressed"  , &lua::L_isLeftMouseButtonPressed)
        .addFunction("isRightMouseButtonPressed" , &lua::L_isRightMouseButtonPressed)
        .addFunction("isMiddleMouseButtonPressed", &lua::L_isMiddleMouseButtonPressed)
        .addFunction("isInputBlocked"            , &lua::L_isInputBlocked)
        .addFunction("blockInput"                , &lua::L_blockInput)

        // GUI
        .addFunction("getInterfacePage"          , &lua::L_getInterfacePage)
        .addFunction("getComponentAbstract"      , &lua::L_getComponentAbstract)
        .addFunction("getComponentWidget"        , &lua::L_getComponentWidget)
        .addFunction("getComponentButton"        , &lua::L_getComponentButton)
        .addFunction("getComponentLabel"         , &lua::L_getComponentLabel)
        .addFunction("getComponentImageList"     , &lua::L_getComponentImageList)
        
        // Miscellaneous    
        .addFunction("getRegionIndex"            , &lua::L_getRegionIndex)
        .addFunction("getTileIndex"              , &lua::L_getTileIndex)
        .addFunction("getRegion"                 , &lua::L_getRegion)
        .addFunction("inWorldBounds"             , &lua::L_inWorldBounds)
        .addFunction("inRegionBounds"            , &lua::L_inRegionBounds)
        .addFunction("tileGridPosition"          , &lua::L_tileGridPosition)
        .addFunction("produce"                   , &lua::L_tickProduction)
        .addFunction("harvest"                   , &lua::L_tickHarvest)
        // World generation
        .addFunction("generateRegion"            , &lua::L_generateRegion)
        .addFunction("generateWorld"             , &lua::L_generateWorld)

        // Player
        .addFunction("getPlayer"                 , &lua::L_getPlayer)
        .addFunction("getHumanPlayer"            , &lua::L_getHumanPlayer)
        .addFunction("isHumanPlayer"             , &lua::L_isHumanPlayer)

        // Game data
        .beginNamespace("gamedata")
            .addVariable("BUILDING_TABLE", &BUILDING_TABLE        , false)
            .addVariable("BIOME_TABLE"   , &BIOME_TABLE           , false)
            .addVariable("ITEM_TABLE"    , &ITEM_TABLE            , false)
            .addVariable("RESOURCE_TABLE", &RESOURCE_TABLE        , false)
            .addVariable("WORLD_MAP"     , &game_manager.world_map, false)
            .addVariable("WORLD_WIDTH"   , &world_data.w_width)
            .addVariable("REGION_WIDTH"  , &world_data.r_width)
            .addVariable("TILE_SIZE"     , &world_data.tile_size)
        .endNamespace()
    ;
}

void L_loadTexture(const std::string& filename, const std::string& id, core::Vector2i position, core::Vector2i size) {
    if(!game_manager.resource.checkTextureExists(id))
        game_manager.resource.loadTexture(filename, id, sf::IntRect(position.asSFMLVector2i(), size.asSFMLVector2i()));
}

void L_loadFont(const std::string& filename, const std::string& id) {
    if(!game_manager.resource.checkFontExists(id))
        game_manager.resource.loadFont(filename, id);
}

void L_loadEvent(const std::string& id, float required_time, bool speed_dependant) {  
    EventData data;
    data.name = id;
    data.time = 0;
    data.required_time = required_time;
    data.speed_dependant = speed_dependant;

    ScheduledEvent event(data);
    game_manager.schedule.push_back(event);
}

void L_createIcon(const std::string& id) {
    if(!game_manager.resource.checkTextureExists(id)) {
        printError("L_createIcon()", "Tried to create icon from not existing texure '" + id + "'");
        return;
    }

    auto save_as = "icon_" + readAfter(id, "icon_template_");
    auto base    = "icon_default";
    game_manager.resource.blendTextures(save_as, base, id); 
}

void L_setGamestate(const std::string& id) {
    game_manager.gamestate.setGamestate(id);
}

void L_showInterface(const std::string& id) {
    auto gamestate = game_manager.gamestate.getGamestate();
    gamestate->setVisibilityTrue(id);
}

void L_hideInterface(const std::string& id) {
    auto gamestate = game_manager.gamestate.getGamestate();
    gamestate->setVisibilityFalse(id);
}

bool L_isInterfaceVisible(const std::string& id) {
    auto gamestate = game_manager.gamestate.getGamestate();
    return gamestate->isComponentVisible(id);
}

bool L_isKeyPressed(const std::string& key_id) {
    auto current_gamestate = game_manager.gamestate.getGamestate();
    return current_gamestate->controls.isKeyPressed(key_id);
}

bool L_isLeftMouseButtonPressed() {
    auto current_gamestate = game_manager.gamestate.getGamestate();
    return current_gamestate->controls.mouse_left;
}

bool L_isRightMouseButtonPressed() {
    auto current_gamestate = game_manager.gamestate.getGamestate();
    return current_gamestate->controls.mouse_right;
}

bool L_isMiddleMouseButtonPressed() {
    auto current_gamestate = game_manager.gamestate.getGamestate();
    return current_gamestate->controls.mouse_middle;
}

gui::InterfacePage* L_getInterfacePage(const std::string& page_id) {
    auto current_gamestate = game_manager.gamestate.getGamestate();
    auto page = current_gamestate->getInterfaceComponent(page_id);
    
    if(page == nullptr) {
        printError("L_getInterfacePage()", "There is no page '" + page_id + "'");
        return nullptr;
    }

    return page;
}

gui::AbstractWidget* L_getComponentAbstract(const std::string& page_id, const std::string& widget_id) {
    auto current_gamestate = game_manager.gamestate.getGamestate();
    auto page = current_gamestate->getInterfaceComponent(page_id);
    auto component = page->getComponent(widget_id);

    if(component == nullptr) {
        printError("L_getComponentAbstract()", "There is no component '" + widget_id + "' in interface page '" + page_id + "'");
        return nullptr;
    }

    auto widget = (gui::AbstractWidget*)(component.get());
    return widget;
}

gui::Widget* L_getComponentWidget(const std::string& page_id, const std::string& widget_id) {
    auto current_gamestate = game_manager.gamestate.getGamestate();
    auto page = current_gamestate->getInterfaceComponent(page_id);
    auto component = page->getComponent(widget_id);

    if(component == nullptr) {
        printError("L_getComponentWidget()", "There is no component '" + widget_id + "' in interface page '" + page_id + "'");
        return nullptr;
    }

    auto widget = (gui::Widget*)(component.get());
    return widget;
}

gui::Button* L_getComponentButton(const std::string& page_id, const std::string& button_id) {
    auto current_gamestate = game_manager.gamestate.getGamestate();
    auto page = current_gamestate->getInterfaceComponent(page_id);
    auto component = page->getComponent(button_id);

    if(component == nullptr) {
        printError("L_getComponentButton()", "There is no component '" + button_id + "' in interface page '" + page_id + "'");
        return nullptr;
    }

    auto button = (gui::Button*)(component.get());
    return button;
}

gui::Label* L_getComponentLabel(const std::string& page_id, const std::string& label_id) {
    auto current_gamestate = game_manager.gamestate.getGamestate();
    auto page = current_gamestate->getInterfaceComponent(page_id);
    auto component = page->getComponent(label_id);

    if(component == nullptr) {
        printError("L_getComponentLabel()", "There is no component '" + label_id + "' in interface page '" + page_id + "'");
        return nullptr;
    }

    auto label = (gui::Label*)(component.get());
    return label;
}

gui::ImageList* L_getComponentImageList(const std::string& page_id, const std::string& list_id) {
    auto current_gamestate = game_manager.gamestate.getGamestate();
    auto page = current_gamestate->getInterfaceComponent(page_id);
    auto component = page->getComponent(list_id);

    if(component == nullptr) {
        printError("L_getComponentImageList()", "There is no component '" + list_id + "' in interface page '" + page_id + "'");
        return nullptr;
    }

    auto list = (gui::ImageList*)(component.get());
    return list;
}

int L_getFPS() {
    return game_manager.fps;
}

float L_getFrameTime() {
    return game_manager.time_per_frame;
}

int L_getRegionIndex() {
    auto* gamestate = game_manager.gamestate.getGamestate();
    if(gamestate->state_id == "Worldmap") {
        auto worldmap = (Worldmap*)gamestate;
        return worldmap->L_getCurrentIndex();
    } 

    else {
        auto regionmap = (Regionmap*)gamestate;
        return regionmap->L_getRegionIndex();
    }
}

int L_getTileIndex() {
    auto* gamestate = game_manager.gamestate.getGamestate();
    if(gamestate->state_id != "Regionmap") {
        printError("L_getTileIndex()", "Current gamestate is not Regionmap");
        return -1;
    }

    auto regionmap = (Regionmap*)gamestate;
    return regionmap->L_getTileIndex();
}

bool L_isBuildingTile(int tile_index) {
    auto* gamestate = game_manager.gamestate.getGamestate();
    if(gamestate->state_id != "Regionmap") {
        printError("L_isBuildingTile()", "Current gamestate is not Regionmap");
        return false;
    }
    
    auto index  = L_getRegionIndex();
    if(!inRegionBounds(index))
        return false;

    auto region = game_manager.world_map.at(index);
    return region.L_isTileOccupied(tile_index);
}

core::Vector2i L_getMousePosition() {
    auto* gamestate = game_manager.gamestate.getGamestate();
    return gamestate->mouse_position_window;
}

core::Vector2i L_getMousePositionInterface() {
    auto* gamestate = game_manager.gamestate.getGamestate();
    return gamestate->mouse_position_interface;
}

void L_exitApplication(int code) {
    exitApplication(code);
}

Region* L_getRegion(int region_index) {
    if(!inWorldBounds(region_index)) {
        printError("L_getRegionIndex()", "Region index out of bounds");
        return nullptr;
    }

    return &game_manager.world_map.at(region_index);
}

Gamestate* L_getCurrentGamestate() {
    auto* gamestate = game_manager.gamestate.getGamestate();
    return gamestate;
}

Gamestate* L_getGamestate(const std::string& state_id) {
    auto* gamestate = game_manager.gamestate.getGamestateByName(state_id);
    return gamestate;
} 

bool L_inWorldBounds(int region_index) {
    return inWorldBounds(region_index);
}

bool L_inRegionBounds(int tile_index) {
    return inRegionBounds(tile_index);
}

core::Vector2i L_tileGridPosition(int tile_index) {
    auto grid = tileGridPosition(tile_index);
    return grid;
}

void L_generateRegion(int region_index) {
    game_manager.world_generator.generateRegion(region_index);
}

void L_generateWorld() {
    game_manager.world_generator.generate();
}

bool L_mouseIntersectsUI() {
    auto gamestate = game_manager.gamestate.getGamestate();
    return gamestate->mouseIntersectsUI();
}

std::string L_getInterfacePageUnderMouse() {
    auto gamestate = game_manager.gamestate.getGamestate();
    if(!gamestate->mouseIntersectsUI())
        return "";

    auto mouse_position = gamestate->mouse_position_interface;
    for(const auto& pair : gamestate->interface) {
        const auto& component = pair.second;
        if(component.get()->intersectsUI(mouse_position) && component.get()->isVisible()) 
            return component.get()->getWidgetID();
    }

    return "";
}

std::string L_getInterfaceItemUnderMouse(const std::string& page_id) {
    auto gamestate = game_manager.gamestate.getGamestate();
    if(!gamestate->checkComponentExist(page_id))
        return "";

    auto page = gamestate->getInterfaceComponent(page_id);
    auto current_component = page->getCurrentComponent();
    return current_component.get() == nullptr
        ? ""
        : current_component.get()->getWidgetID();
}

Regionmap* L_getRegionmap() {
    auto regionmap = (Regionmap*)game_manager.gamestate.getGamestateByName("Regionmap");
    return regionmap;
}

Worldmap* L_getWorldmap() {
    auto worldmap = (Worldmap*)game_manager.gamestate.getGamestateByName("Worldmap");
    return worldmap;
}

core::Vector2i L_getWindowSize() {
    return game_manager.window.getWindowSize();
}

int L_getWindowWidth() {
    return game_manager.window.getWindowWidth();
}

int L_getWindowHeight() {
    return game_manager.window.getWindowHeight();
}

bool L_inScreenSpace(const iso::GameObject& gameobject) {
    return game_manager.window.inScreenSpace(gameobject);
}

void L_drawGameObject(iso::GameObject& gameobject) {
    sf::RenderStates states;
    states.texture = &game_manager.resource.getTexture(gameobject.getTextureName());
    
    auto gamestate = game_manager.gamestate.getGamestate();
    auto view = game_manager.window.getView();
    game_manager.window.setView(gamestate->view_game);
    game_manager.window.draw(gameobject, states);
    game_manager.window.setView(view);
}

void L_drawBuilding(iso::Building& building) {
    sf::RenderStates states;
    states.texture = &game_manager.resource.getTexture(building.getTextureName());
    
    auto gamestate = game_manager.gamestate.getGamestate();
    auto view = game_manager.window.getView();
    game_manager.window.setView(gamestate->view_game);
    game_manager.window.draw(building, states);
    game_manager.window.setView(view);
}

void L_setVSync(bool on) {
    game_manager.window.setVsync(on);
}

void L_setMaxFramerate(int framerate) {
    if(framerate < 0)
        iso::printError("L_setMaxFramerate", "Maximum FPS can't be negative");

    game_manager.window.setMaxFramerate(framerate);
}

void L_takeScreenshot() {
    game_manager.window.takeScreenshot();
}

Player* L_getHumanPlayer() {
    return game_manager.getHumanPlayer();
}

Player* L_getPlayer(int player_id) {
    return game_manager.getPlayer(player_id);
}

bool L_isHumanPlayer(int player_id) {
    return game_manager.isHumanPlayer(player_id);
}

bool L_isInputBlocked() {
    auto gamestate = game_manager.gamestate.getGamestate();
    return gamestate->controls.isInputBlocked();
}

void L_blockInput(bool block) {
    auto gamestate = game_manager.gamestate.getGamestate();
    gamestate->controls.blockKeyboardInput(block);
}

void L_tickProduction(int region_index, int tile_index) {
    if(!inWorldBounds(region_index)) {
        printError("L_tickHarvest()", "Region index out of bounds");
        return;
    }

    if(!inRegionBounds(region_index)) {
        printError("L_tickHarvest()", "Tile index out of bounds");
        return;
    }

    auto& region = game_manager.world_map.at(region_index);
    if(!region.buildingExistsAtIndex(tile_index)) {
        printError("L_tickHarvest()", "There is no building at index '" + std::to_string(tile_index) + "'");
        return;
    }  

    const auto& building = region.getBuildingAtIndex(tile_index);
    if(!building.isProductionBuilding())
        return;

    std::map <std::string, int> produced_items;
    auto production = building.getBuildingProduction();
    for(auto production_item : production) {
        auto current_ticks = 0;
        auto required_ticks = production_item.requirements.size();

        // Check if the production requirements are met.
        for(auto resource : production_item.requirements) {
            if(region.itemExists(resource) && region.itemQuantity(resource) >= resource.getAmount())
                current_ticks++;
        }

        // If production requirements are met, make the item.
        if(current_ticks == required_ticks) {
            for(auto resource : production_item.requirements)
                region.stockpileRemove(resource);

            for(auto item : ITEM_TABLE) {
                if(production_item.name == item.getItemName()) {
                    item.setAmount(production_item.amount);
                    region.stockpileAdd(item);
                    printWarning("production", "Produced: " + item.getItemName() + " " + std::to_string(item.getAmount()));
                    break;
                }
            }   
        }
    }
}

void L_tickHarvest(int region_index, int tile_index) {
    if(!inWorldBounds(region_index)) {
        printError("L_tickHarvest()", "Region index out of bounds");
        return;
    }

    if(!inRegionBounds(region_index)) {
        printError("L_tickHarvest()", "Tile index out of bounds");
        return;
    }

    auto& region = game_manager.world_map.at(region_index);
    if(!region.buildingExistsAtIndex(tile_index)) {
        printError("L_tickHarvest()", "There is no building at index '" + std::to_string(tile_index) + "'");
        return;
    }    

    const auto& building = region.getBuildingAtIndex(tile_index);
    if(!building.isHarvestBuilding())
        return;
    
    std::map <std::string, int> harvested_items;
    for(int y = -building.getBuildingArea().y; y <= building.getBuildingArea().y + 1; y++) {
        for(int x = -building.getBuildingArea().x; x <= building.getBuildingArea().x + 1; x++) {
            auto index = tile_index + calculateRegionIndex(x, y);
            
            if(building.isTileHarvestable(region_index, index)) {
                for(const auto& harvested_resource : building.getBuildingHarvests()) {
                    if(region.treeExistsAt(index) && harvested_resource.name == "Tree")
                        harvested_items["Wood"]++;

                    else if(region.getResourceAt(tile_index).getResourceName() == harvested_resource.name)
                        harvested_items[harvested_resource.name]++;
                }
            }
        }
    }

    std::vector <StorageItem> resources_as_items;
    for(auto harvested_item : harvested_items) {
        for(auto item : ITEM_TABLE) {
            if(harvested_item.first == item.getItemName() && harvested_item.second ) {
                item.setAmount(harvested_item.second);
                resources_as_items.push_back(item);
                printWarning("harvest", "Harvested: " + item.getItemName() + std::to_string(item.getAmount()));
            }
        }
    }

    for(auto item : resources_as_items)
        region.stockpileAdd(item);
}
}
