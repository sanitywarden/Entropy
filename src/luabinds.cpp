#include "luabinds.hpp"
#include "simulationManager.hpp"
#include "globalutilities.hpp"
#include "gamestate.hpp"

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
            .addProperty("a", core::Colour::getA, core::Colour::setR)
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
            .addProperty("y", core::Vector3f::getZ, core::Vector3f::setZ) 
        .endClass()
        .beginClass <core::Vector3i> ("Vector3i")
            .addConstructor <void (*) (float, float, float)> ()
            .addProperty("x", core::Vector3i::getX, core::Vector3i::setX) 
            .addProperty("y", core::Vector3i::getY, core::Vector3i::setY) 
            .addProperty("y", core::Vector3i::getZ, core::Vector3i::setZ) 
        .endClass()
        .beginClass <core::Ratio> ("Ratio")
            .addConstructor <void (*) (std::string, std::string)> ()
        .endClass()
        .beginClass <Building> ("Building")
            .addConstructor <void (*) ()> ()
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
        .endClass()
        .beginClass <Biome> ("Biome")
            .addConstructor <void (*) ()> ()
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
        .beginClass <Region> ("Region")
            .addFunction("getTileAtIndex", &Region::getTileAtIndex)
            .addFunction("isBiome"       , &Region::isBiome)
        .endClass()
        .beginClass <Tile> ("Tile")
            .addFunction("isTerrain", &Tile::isTerrain)
            .addFunction("isWater"  , &Tile::isWater)
            .addFunction("isOcean"  , &Tile::isOcean)
            .addFunction("isRiver"  , &Tile::isRiver)
            .addFunction("isCoast"  , &Tile::isCoast)
        .endClass()
        .beginClass <gui::Label> ("Label")
            .addProperty("text"  , &gui::Label::getString, &gui::Label::setString)
            .addProperty("colour", &gui::Label::getColour, &gui::Label::setColour)
            .addProperty("font"  , &gui::Label::getFont  , &gui::Label::setFont)
        .endClass()
        .addFunction("loadTexture"           , &lua::L_loadTexture)
        .addFunction("loadFont"              , &lua::L_loadFont)
        .addFunction("loadScheduledEvent"    , &lua::L_loadEvent)
        .addFunction("createIcon"            , &lua::L_createIcon)
        .addFunction("setGamestate"          , &lua::L_setGamestate)
        .addFunction("isKeyPressed"          , &lua::L_isKeyPressed)
        .addFunction("showInterface"         , &lua::L_showInterface)
        .addFunction("hideInterface"         , &lua::L_hideInterface)
        .addFunction("isInterfaceVisible"    , &lua::L_isInterfaceVisible)
        .addFunction("getComponentLabel"     , &lua::L_getComponentLabel)
        .addFunction("getFPS"                , &lua::L_getFPS)
        .addFunction("getFrameTime"          , &lua::L_getFrameTime)
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

    const auto& texture = game_manager.resource.getTexture(id);
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

int L_getFPS() {
    return game_manager.fps;
}

float L_getFrameTime() {
    return game_manager.time_per_frame;
}
}
