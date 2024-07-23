---@type Scene
local mainMenu = {}

local colors = require("api.ui.colors")
local layout = require("api.ui.layout")
local styles = require("api.ui.styles")
local utils = require("api.utils")

mainMenu.ui = {
    style = {
        alignItems = layout.items.LAY_COLUMN,
        alignSelf = layout.self.LAY_TOP_LEFT,
        margin = 0.16,
    },
    {
        style = {
            alignItems = layout.items.LAY_ROW,
        },
        {
            "Party",
            style = {
                color = colors.text_color,
                fontSize = 200,
                marginRight = 0.02,
            },
        },
        {
            "Lab",
            style = {
                color = colors.accent_color,
                fontSize = 200,
            },
        },
    },
    {
        style = {
            alignItems = layout.items.LAY_COLUMN,
            alignSelf = layout.self.LAY_LEFT,
        },
        {
            type = "button",
            text = "Join Game",
            style = styles.defaultButton,
            onClick = function()
                utils.info("Join")
            end
        },
        {
            type = "button",
            text = "Host Game",
            style = styles.defaultButton,
            onClick = function()
                utils.info("Host")
            end
        },
        {
            type = "button",
            text = "Quit",
            style = styles.defaultButton,
            onClick = function()
                require("api.core").exit()
            end
        },
    },
}

return mainMenu