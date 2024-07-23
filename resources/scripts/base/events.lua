BASE_SCENES = {}

local m = {
    addScenes = function(registrar)
        BASE_SCENES.mainmenu = registrar(require("base.scenes.mainmenu"))
    end
}

return m