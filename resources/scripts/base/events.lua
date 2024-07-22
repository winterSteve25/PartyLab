BASE_SCENES = {}

local log = require("api.utils")

local m = {
    addScenes = function(registrar)
        BASE_SCENES.mainmenu = registrar(require("base.scenes.mainmenu"))
    end
}

return m