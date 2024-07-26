local m = {}

local colors = require("api.ui.colors")
local ease = require("api.ui.ease")

---@type Style
m.defaultButton = {
    alignSelf = require("api.ui.layout").self.LAY_LEFT,
    color = colors.textColor,
    fontSize = 100,
    marginTop = 0.04,
    hovered = {
        color = colors.accentColor,
        fontSize = 130,
    },
    pressed = {
        fontSize = 150,
    },
    transition = {
        color = Transition(100, ease.easeLinear),
        fontSize = Transition(90, ease.easeOutExpo)
    },
}

return m