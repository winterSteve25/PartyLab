BASE_SCENES = {}

local m = {
    addScenes = function(registrar)
        BASE_SCENES.mainmenu = registrar(require("base.scenes.mainmenu"))
        BASE_SCENES.lobby = registrar(require("base.scenes.lobby"))
    end,
}

return m