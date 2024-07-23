local m = {}

local colors = require("api.ui.colors")
local ease = require("api.ui.ease")

---@type Style
m.defaultButton = {
    alignSelf = require("api.ui.layout").self.LAY_LEFT,
    color = colors.text_color,
    fontSize = 96,
    hovered = {
        color = colors.accent_color,
        fontSize = 128,
    },
    pressed = {
        fontSize = 140,
    },
    transition = {
        color = Transition(100, ease.easeLinear),
        fontSize = Transition(90, ease.easeOutExpo)
    },
}

return m