TransitionData = {
    lobbyStatus = nil,
    joining = nil,
}

LobbyData = {
    isEveryoneReady = false,
    ready = {},
    gameMode = 1,
}

function resetLobbyData()
    LobbyData = {
        isEveryoneReady = false,
        ready = {},
        gameMode = 1,
    }
end

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