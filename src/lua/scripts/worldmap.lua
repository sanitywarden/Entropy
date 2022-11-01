Script = {
    onBuildingConstruct = function()
        print("Building placed")
    end,

    onBuildingDestroy = function()
        print("Building destroyed")
    end,

    onProductionTick = function()
        print("Production tick")
    end
}