---@type GlobalEventHandler
local m = {}

m.addScenes = function(registrar)
    SCENES.mainmenu = registrar(require("base.scenes.mainmenu"))
    SCENES.lobby = registrar(require("base.scenes.lobby"))
    SCENES.waitingLobby = registrar(require("base.scenes.waitingLobby"))
end

m.addPackets = function(registrar)
    PACKETS.readyToggle = registrar(require("base.packets.readytoggle"))
    PACKETS.chooseGamemode = registrar(require("base.packets.chooseGamemode"))
end

m.addGameModes = function(registrar) 
    registrar({
        name = "Word Mania 1",
        description = "Jumbo the words",
        iconLocation = "resources/Word Mania.png",
        maxPlayers = 8,
        minPlayers = 2,
        scene = SCENES.mainmenu,
    })

    registrar({
        name = "Word Mania 2",
        description = "Jumbo the words",
        iconLocation = "resources/Word Mania.png",
        maxPlayers = 8,
        minPlayers = 2,
        scene = SCENES.mainmenu,
    })

    registrar({
        name = "Word Mania 3",
        description = "Jumbo the words",
        iconLocation = "resources/Word Mania.png",
        maxPlayers = 8,
        minPlayers = 2,
        scene = SCENES.mainmenu,
    })
end

return m