BASE_SCENES = {}

---@type GlobalEventHandler
local m = {}

m.addScenes = function(registrar)
    BASE_SCENES.mainmenu = registrar(require("base.scenes.mainmenu"))
    BASE_SCENES.lobby = registrar(require("base.scenes.lobby"))
    BASE_SCENES.hosting = registrar(require("base.scenes.hosting"))
end

return m