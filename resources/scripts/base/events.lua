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

m.addGameModes = function(registrar) 
    registrar({
        name = "Word Mania",
        description = "Jumbo the words",
        iconLocation = "resources/Word Mania.png",
        maxPlayers = 8,
        minPlayers = 2,
        scene = SCENES.mainmenu,
    })

    registrar({
        name = "Word Mania",
        description = "Jumbo the words",
        iconLocation = "resources/Word Mania.png",
        maxPlayers = 8,
        minPlayers = 2,
        scene = SCENES.mainmenu,
    })

    registrar({
        name = "Word Mania",
        description = "Jumbo the words",
        iconLocation = "resources/Word Mania.png",
        maxPlayers = 8,
        minPlayers = 2,
        scene = SCENES.mainmenu,
    })
end

return m