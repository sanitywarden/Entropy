Script = {
    onKeyPress = function()
        gamestate = getCurrentGamestate()
        if gamestate:getID() ~= "Worldmap" or isInputBlocked() then
            return
        end

        if isKeyPressed("ESCAPE") then
            exitApplication(0)
        end

        if isKeyPressed("R") then
            generateWorld()
        end

        if isKeyPressed("F12") then
            takeScreenshot()
        end
    end
}