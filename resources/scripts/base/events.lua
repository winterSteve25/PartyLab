---@type GlobalEventHandler
local m = {}

m.addScenes = function(registrar)
    SCENES.mainmenu = registrar(require("base.scenes.mainmenu"))
    SCENES.join = registrar(require("base.scenes.join"))
    SCENES.lobby = registrar(require("base.scenes.lobby"))
    SCENES.waitingLobby = registrar(require("base.scenes.waitingLobby"))
    
    SCENES.game_fortunatelyUnfortunately = registrar(require("base.scenes.games.fortunatelyunfortunately.game"))
end

m.addPackets = function(registrar)
    PACKETS.readyToggle = registrar(require("base.packets.readytoggle"))
    PACKETS.handshake = registrar(require("base.packets.handshake"))
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
        name = "Fortunately Unfortunately",
        description = "Starts with fortunately ___, then follow with unfortunately, ____",
        iconLocation = "resources/Word Mania.png",
        maxPlayers = 8,
        minPlayers = 1,
        scene = SCENES.game_fortunatelyUnfortunately,
    })
end

return m