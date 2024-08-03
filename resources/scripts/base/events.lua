---@type GlobalEventHandler
local m = {}

m.addScenes = function(registrar)
    SCENES.mainmenu = registrar(require("base.scenes.mainmenu"))
    SCENES.lobby = registrar(require("base.scenes.lobby"))
    SCENES.waitingLobby = registrar(require("base.scenes.waitingLobby"))
end

m.addPackets = function(registrar)
    PACKETS.readyToggle = registrar(require("base.packets.readytoggle"))
end

return m