local mainMenu = {}

local colors = require("api.colors")
local layout = require("api.layout")
local utils = require("api.utils")

mainMenu.load = function()
end

---@type UI
mainMenu.ui = {
    style = {
        alignItems = layout.items.LAY_ROW,
        marginTop = 0.4,
        marginLeft = 0.2,
    },
    {
        "Party",
        style = {
            color = colors.text_color,
            fontSize = 128,
            transition = {
                color = Transition(100, function(t)
                    return t ^ 2
                end),
            },
            hovered = {
                color = colors.accent_color,
            },
        },
    },
    {
        "Lab",
        style = {
            color = colors.text_color,
            fontSize = 128,
        },
    },
}

--mainMenu.ui = {
--    "Party",
--    style = {
--        color = colors.text_color,
--        fontSize = 128,
--        transition = {
--            color = Transition(100, function(t)
--                return t ^ 2
--            end),
--        },
--        hovered = {
--            color = colors.accent_color,
--        },
--    },
--}

return mainMenu