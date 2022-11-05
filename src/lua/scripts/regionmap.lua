require "src.lua.data.stringutil"

Script = {
    onProductionTick = function()
        print("Regionmap: Production tick")          
    end,
    onHarvestTick = function()
        print("Regionmap: Harvest tick")

        local regions = gamedata.WORLD_MAP
        for i = 1, rawlen(regions), 1 do
            local region = regions[i]
            if region:hasOwner() then
                local buildings = region:getBuildingList()
                if rawlen(buildings) ~= 0 then
                    for j = 1, rawlen(buildings), 1 do
                        building = buildings[j]
                        if building.name ~= "Empty" and building:isHarvestBuilding() then
                            harvest(region.index, building.index)
                        end
                    end 
                end
            end
        end   
    end
}