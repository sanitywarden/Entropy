#ifndef _ITEM_HPP_
#define _ITEM_HPP_

#include <string>
#include <vector>

namespace iso {
    // Classification of items held in region's storage.
    enum class ItemType {
        TYPE_UNCATEGORISED,
        TYPE_FOOD,             // Apple, Pear etc.
        TYPE_DRINKABLE_LIQUID, // Water, Beer etc.
        TYPE_BASE_MATERIAL,    // Wood, Stone etc.
    };

    // Item is a commodity held in a region's storage.
    // Item may be extracted from a resource or created from other items.
    class StorageItem {
        public:
            int         quantity;
            ItemType    item_type;
            std::string item_name;
            std::string item_icon;

            StorageItem();
            StorageItem(const std::string& name, int quantity, ItemType type);
            StorageItem(const std::string& name, int quantity, ItemType type, const std::string& icon);
            ~StorageItem();
            
            StorageItem& operator -= (const StorageItem& item);
            StorageItem  operator -  (const StorageItem& item);  
            StorageItem& operator += (const StorageItem& item);
            StorageItem  operator +  (const StorageItem& item);  
            bool operator >  (const StorageItem& item) const;
            bool operator >= (const StorageItem& item) const;
            bool operator <  (const StorageItem& item) const;
            bool operator <= (const StorageItem& item) const;
    };

    const StorageItem ITEM_EMPTY  ("None"   , 0, ItemType::TYPE_UNCATEGORISED   , "icon_default"     );
    const StorageItem ITEM_WOOD   ("Wood"   , 0, ItemType::TYPE_BASE_MATERIAL   , "icon_item_wood"   );
    const StorageItem ITEM_STONE  ("Stone"  , 0, ItemType::TYPE_BASE_MATERIAL   , "icon_item_stone"  );
    const StorageItem ITEM_FLINT  ("Flint"  , 0, ItemType::TYPE_BASE_MATERIAL   , "icon_item_flint"  );
    const StorageItem ITEM_GRAIN  ("Grain"  , 0, ItemType::TYPE_FOOD            , "icon_item_grain"  );
    const StorageItem ITEM_MEAT   ("Meat"   , 0, ItemType::TYPE_FOOD            , "icon_item_meat"   );
    const StorageItem ITEM_LEATHER("Leather", 0, ItemType::TYPE_BASE_MATERIAL   , "icon_item_leather");
    const StorageItem ITEM_WATER  ("Water"  , 0, ItemType::TYPE_DRINKABLE_LIQUID, "icon_item_water"  );
    const StorageItem ITEM_TOOLS  ("Tools"  , 0, ItemType::TYPE_BASE_MATERIAL   , "icon_item_tools"  );

    const static std::vector <StorageItem> ITEM_LOOKUP_TABLE = {
        ITEM_WOOD,
        ITEM_STONE,
        ITEM_FLINT,
        ITEM_GRAIN,
        ITEM_MEAT,
        ITEM_LEATHER,
        ITEM_WATER,
        ITEM_TOOLS,
    };
}

#endif