
base_scenes = {}

local log = require("api.utils")
local m = {
    addScenes = function(registrar)
        base_scenes.mainmenu = registrar(require("base.scenes.mainmenu"))
    end
}

return m