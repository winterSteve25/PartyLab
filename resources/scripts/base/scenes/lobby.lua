---@type Scene
local m = {}

local layout = require("api.ui.layout")
local styles = require("api.ui.styles")
local ease = require("api.ui.ease")
local colors = require("api.ui.colors")
local utils = require("api.utils")

---@type Style
local textStyle = {
    color = colors.backgroundColor,
    fontSize = 70,
    marginRight = 0.015,
    marginTop = 0.025,
    marginBottom = 0.025,
}

---@return UI
local function leftPanel(data)
    return {
        style = {
            alignItems = bit32.bor(layout.items.LAY_COLUMN, layout.items.LAY_START),
            alignSelf = bit32.bor(layout.self.LAY_VFILL, layout.self.LAY_LEFT),
            width = 0.4,
            backgroundColor = colors.middlegroundColor,
        },
        {
            style = {
                alignSelf = layout.self.LAY_HFILL,
                alignItems = bit32.bor(layout.items.LAY_LAYOUT),
                backgroundColor = colors.foregroundColor,
            },
            {
                "Game Mode",
                style = utils.mergeTables(textStyle, {
                    alignSelf = layout.self.LAY_CENTER,
                }),
            },
            {
                type = "button",
                text = "Settings",
                style = utils.mergeTables(styles.defaultButton, {
                    color = colors.backgroundColor:a(200),
                    alignSelf = layout.self.LAY_RIGHT,
                    fontSize = 60,
                    marginLeft = 0,
                    marginTop = 0,
                    marginBottom = 0,
                    marginRight = 0.015,
                    hovered = {
                        color = colors.backgroundColor,
                        fontSize = 66,
                    },
                    pressed = {
                        fontSize = 70,
                    }
                }),
                onClick = function()
                end
            },
        },
        {
            type = "rendered",
            style = {},
            render = function()
                local settingsOpen = utils.getOr(data, "settings", false)

                if settingsOpen then
                else
                end
            end
        }
    }
end

---@return UI
local function rightPanel(data)
    return {
        style = {
            alignSelf = layout.self.LAY_FILL,
            alignItems = bit32.bor(layout.items.LAY_COLUMN, layout.items.LAY_START),
            marginLeft = 30,
        },
        {
            style = {
                height = 400,
                backgroundColor = colors.middlegroundColor,
                alignSelf = layout.self.LAY_HFILL,
                alignItems = bit32.bor(layout.items.LAY_COLUMN, layout.items.LAY_START),
            },
            {
                style = {
                    backgroundColor = colors.foregroundColor,
                    alignSelf = layout.self.LAY_HFILL,
                    alignItems = bit32.bor(layout.items.LAY_ROW, layout.items.LAY_START),
                },
                {
                    "Players",
                    style = utils.mergeTables(textStyle, {
                        marginLeft = 0.015,
                    })
                },
            },
        },
        {

        }
    }
end

m.ui = function(data)
    return {
        style = {
            alignItems = layout.items.LAY_ROW,
            alignSelf = layout.self.LAY_FILL,
            margin = 0.02,
        },
        leftPanel(data),
        rightPanel(data),
    }
end

return m