TransitionData = {
    lobbyStatus = nil,
    joining = nil,
}

function resetLobbyData()
    LobbyData = { 
        ready = {},
    }
end

resetLobbyData()

SCENES = {}
PACKETS = {}
LOBBY_STATUS = {
    JOINING = 0,
    HOSTING = 1,
}

---@type ModDescriptor
return {
    name = "Base Game",
    description = "The base games",
    events = require("base.events"),
}