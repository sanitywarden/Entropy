#pragma once

#include "resource.hpp"
#include "stone.hpp"
#include "flint.hpp"
#include "animal.hpp"
#include "copper.hpp"
#include "clay.hpp"

#include <vector>
#include <memory>

namespace iso {
    const Resource RESOURCE_EMPTY  ("None", ResourceType::TYPE_UNCATEGORISED, "icon_default");

    const Stone  RESOURCE_STONE;
    const Flint  RESOURCE_FLINT;
    const Animal RESOURCE_ANIMAL;
    const Copper RESOURCE_COPPER;
    const Clay   RESOURCE_CLAY;

    const static std::vector <std::shared_ptr<Resource>> RESOURCE_LOOKUP_TABLE = {
        std::shared_ptr <Resource> (new Stone()),
        std::shared_ptr <Resource> (new Flint()),
        std::shared_ptr <Resource> (new Animal()),
        std::shared_ptr <Resource> (new Copper()),
        std::shared_ptr <Resource> (new Clay()),
    };

    std::shared_ptr <Resource> getResourcePointer(const std::string& resource_name);

    // TODO: Write files for these.
    
    // const Resource RESOURCE_CLAY   ("Clay"   , ResourceType::TYPE_BASE_MATERIAL   , "icon_item_clay"  );
    // const Resource RESOURCE_COPPER ("Copper" , ResourceType::TYPE_RAW_MATERIAL    , "icon_item_copper");
    // const Resource RESOURCE_TIN    ("Tin"    , ResourceType::TYPE_RAW_MATERIAL    , "icon_item_tin"   );
    // const Resource RESOURCE_COAL   ("Coal"   , ResourceType::TYPE_RAW_MATERIAL    , "icon_item_coal"  );
    // const Resource RESOURCE_IRON   ("Iron"   , ResourceType::TYPE_RAW_MATERIAL    , "icon_item_iron"  );
    
    // const Resource RESOURCE_APPLE  ("Apple"  , ResourceType::TYPE_PLANT_FOOD      , "icon_item_apple" );
    // const Resource RESOURCE_CHERRY ("Cherry" , ResourceType::TYPE_PLANT_FOOD      , "icon_item_cherry");
    // const Resource RESOURCE_GRAPE  ("Grape"  , ResourceType::TYPE_PLANT_FOOD      , "icon_item_grape" );
    // const Resource RESOURCE_POTATO ("Potato" , ResourceType::TYPE_PLANT_FOOD      , "icon_item_potato");
    // const Resource RESOURCE_CARROT ("Carrot" , ResourceType::TYPE_PLANT_FOOD      , "icon_item_carrot");
    // const Resource RESOURCE_WHEAT  ("Wheat"  , ResourceType::TYPE_PLANT_FOOD      , "icon_item_wheat" );
    // const Resource RESOURCE_COTTON ("Cotton" , ResourceType::TYPE_PLANT_RESOURCE  , "icon_item_cotton");
    // const Resource RESOURCE_FLAX   ("Flax"   , ResourceType::TYPE_PLANT_RESOURCE  , "icon_item_flax"  );
}