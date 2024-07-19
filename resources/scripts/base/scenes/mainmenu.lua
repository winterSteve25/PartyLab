local mainMenu = {}
local utils = require("api.utils")
local colors = require("api.colors")

mainMenu.renderOverlay = function()
    utils.drawText("Party", 128.0, Vector2(100, 100), colors.text_color)
    utils.drawText("Lab", 128.0, Vector2(320, 100), colors.accent_color)
end

return mainMenu